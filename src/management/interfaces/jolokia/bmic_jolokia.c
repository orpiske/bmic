/**
 Copyright 2016 Otavio Rodolfo Piske

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */
#include "bmic_jolokia.h"

/**
 * Formats the path to read an attribute
 * @param obj
 * @param name
 * @param status
 * @return
 */
static const char *bmic_jolokia_cap_attr_path(
	const bmic_object_t *obj, const char *name, gru_status_t *status) {
	char *ret;

	int rc = asprintf(&ret, "/value/%s/attr/%s", obj->name, name);

	if (rc == -1) {
		gru_status_set(
			status, GRU_FAILURE, "Not enough memory to format capabilities path");

		return NULL;
	}

	return ret;
}

const bmic_cap_info_t *bmic_jolokia_read_attr_info(
	const bmic_object_t *capabilities, const char *attr_name, gru_status_t *status) {
	/*
	 * Gets the path to the attribute???
	 *
	 * ie.:
	 * /value/address=\"queue.name\",brokerName=\"0.0.0.0\",module=Core,name=\"queue.name\",serviceType=Queue,type=Broker/attr/MessageCount
	 *
	 */
	const char *rev = bmic_jolokia_cap_attr_path(capabilities, attr_name, status);
	if (!rev) {
		return NULL;
	}

	/*
	 * Find the value object for the capability/attribute
	 */
	const bmic_object_t *value_attributes = bmic_object_find_by_path(capabilities, rev);
	gru_dealloc_string((char **) &rev);
	if (!value_attributes) {
		gru_status_set(status, GRU_FAILURE,
			"Unable to find a capability/attribute named %s", attr_name);

		return NULL;
	}

	/*
	 * Read the attributes (ie.: whether it's rw, type, desc) of the value
	 * (ie.: attributes of the requested capability/attribute) and put them
	 * into the bmic_cap_info_t * object
	 */
	bmic_cap_info_t *info = bmic_cap_info_new(status);
	if (!info) {
		gru_status_set(status, GRU_FAILURE,
			"Unable to allocate memory for the capability metadata", attr_name);

		return NULL;
	}

	bmic_cap_info_set_name(info, attr_name);
	bmic_jolokia_translate_attr(value_attributes, info);
	return info;
}

void bmic_jolokia_translate_attr(const bmic_object_t *obj, bmic_cap_info_t *info) {
	const bmic_object_t *rw = bmic_object_find_by_name(obj, JOLOKIA_OBJ_RW_NAME);
	if (rw && rw->type == BOOLEAN) {
		bmic_cap_info_set_write(info, rw->data.value);
	}

	const bmic_object_t *type = bmic_object_find_by_name(obj, JOLOKIA_OBJ_TYPE_NAME);
	if (type && type->type == STRING) {
		bmic_cap_info_set_typename(info, type->data.str);
	}

	const bmic_object_t *desc = bmic_object_find_by_name(obj, JOLOKIA_OBJ_DESC_NAME);
	if (desc && desc->type == STRING) {
		bmic_cap_info_set_description(info, desc->data.str);
	}
}

void bmic_jolokia_translate_attr_object(const void *nodedata, void *payload) {
	const bmic_object_t *nodeobj = (bmic_object_t *) nodedata;
	bmic_payload_add_attr_t *pl = (bmic_payload_add_attr_t *) payload;

	if (nodeobj->type == OBJECT) {
		if (nodeobj->name && strcmp(nodeobj->name, JOLOKIA_OBJ_ATTR_NAME) != 0) {
			bmic_cap_info_t *info = bmic_cap_info_new(pl->status);

			if (!info) {
				return;
			}

			bmic_cap_info_set_name(info, nodeobj->name);
			bmic_jolokia_translate_attr(nodeobj, info);
			gru_list_append(pl->list, info);
		}
	}
}

static void bmic_jolokia_translate_arg_value(const void *nodedata, void *payload) {
	const bmic_object_t *node = (const bmic_object_t *) nodedata;
	bmic_op_sig_t *sig = (bmic_op_sig_t *) payload;

	// TODO: fix this
	bmic_op_arg_t *arg = bmic_op_arg_new(NULL);

	const bmic_object_t *name = bmic_object_find_by_name(node, JOLOKIA_OBJ_NAME_NAME);
	if (name && name->type == STRING) {
		bmic_op_arg_set_name(arg, name->data.str);
	}

	const bmic_object_t *type = bmic_object_find_by_name(node, JOLOKIA_OBJ_TYPE_NAME);
	if (type && type->type == STRING) {
		bmic_op_arg_set_type(arg, type->data.str);
	}

	const bmic_object_t *desc = bmic_object_find_by_name(node, JOLOKIA_OBJ_DESC_NAME);
	if (desc && desc->type == STRING) {
		bmic_op_arg_set_description(arg, desc->data.str);
	}

	bmic_op_sig_add_arg(sig, arg);
}

static void bmic_jolokia_translate_sigs(const void *nodedata, void *payload) {
	const bmic_object_t *node = (const bmic_object_t *) nodedata;
	bmic_op_info_t *info = (bmic_op_info_t *) payload;

	const bmic_object_t *args = bmic_object_find_by_name(node, JOLOKIA_OBJ_ARGS_NAME);
	bmic_op_sig_t *sig = bmic_op_sig_new(NULL);

	if (args && args->type == LIST) {
		bmic_object_for_each_child(args, bmic_jolokia_translate_arg_value, sig);
	}

	const bmic_object_t *ret = bmic_object_find_child_by_name(node, JOLOKIA_OBJ_RET_NAME);
	if (ret && ret->type == STRING) {
		bmic_op_sig_set_ret(sig, ret->data.str);
	}

	const bmic_object_t *desc =
		bmic_object_find_child_by_name(node, JOLOKIA_OBJ_DESC_NAME);
	if (desc && desc->type == STRING) {
		bmic_op_sig_set_description(sig, desc->data.str);
	}

	bmic_op_info_add_signature(info, sig);
}

void bmic_jolokia_translate_op(
	const bmic_object_t *obj, bmic_op_info_t *info, gru_status_t *status) {
	if (obj->type == LIST) {
		bmic_object_for_each_child(obj, bmic_jolokia_translate_sigs, info);
	} else {
		bmic_jolokia_translate_sigs(obj, info);
	}
}

void bmic_jolokia_translate_op_object(const void *nodedata, void *payload) {
	const bmic_object_t *nodeobj = (bmic_object_t *) nodedata;
	bmic_payload_add_attr_t *pl = (bmic_payload_add_attr_t *) payload;

	if (nodeobj->type == OBJECT || nodeobj->type == LIST) {
		if (nodeobj->name && strcmp(nodeobj->name, JOLOKIA_OBJ_OP_NAME) != 0) {
			bmic_op_info_t *info = bmic_op_info_new(pl->status);

			if (!info) {
				return;
			}

			bmic_op_info_set_name(info, nodeobj->name);
			bmic_jolokia_translate_op(nodeobj, info, pl->status);

			gru_list_append(pl->list, info);
		}
	}
}

bool bmic_jolokia_translate_status(const bmic_object_t *root, gru_status_t *status) {
	const bmic_object_t *response_status =
		bmic_object_find_child_by_name(root, JOLOKIA_OBJ_STATUS_NAME);

	if (response_status && response_status->type == INTEGER) {
		if (response_status->data.number != JOLOKIA_STATUS_OK) {
			const bmic_object_t *error =
				bmic_object_find_child_by_name(root, JOLOKIA_OBJ_ERROR_NAME);
			if (error && error->type == STRING) {
				gru_status_set(status, GRU_FAILURE, "Error %d: %s",
					response_status->data.number, error->data.str);
			} else {
				gru_status_set(status, GRU_FAILURE, "Unknown error: %d",
					response_status->data.number);
			}
		}

		logger_t logger = gru_logger_get();

		logger(DEBUG, "Jolokia response status: %d", response_status->data.number);
		return true;
	}

	return false;
}