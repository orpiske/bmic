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
#include "bmic_activemq.h"

static const char *activemq_base_url;
static const bmic_exchange_t *cap_cache;

bmic_api_interface_t *bmic_activemq_product(gru_status_t *status) {
	bmic_api_interface_t *ret =
		bmic_api_interface_new(ACTIVEMQ_PRODUCT_NAME, ACTIVEMQ_API_VERSION, status);

	ret->base_url = bmic_activemq_base_url;
	ret->api_init = bmic_activemq_init;
	ret->api_cleanup = bmic_activemq_cleanup;
	ret->product_info = bmic_activemq_product_info;
	ret->capabilities_load = bmic_activemq_load_capabilities;
	ret->attribute_read = bmic_activemq_attribute_read;
	ret->attribute_list = bmic_activemq_attribute_list;
	ret->queue_attribute_read = bmic_activemq_queue_attribute_read;
	ret->operation_list = bmic_activemq_operation_list;
	ret->queue_create = bmic_activemq_queue_create;
	ret->queue_delete = bmic_activemq_queue_delete;
	ret->queue_list = bmic_activemq_queue_list;
	ret->queue_stats = bmic_activemq_queue_stats;

	ret->java.java_info = bmic_java_read_info;
	ret->java.os_info = bmic_java_read_os_info;

	ret->java.eden_info = bmic_java_mem_eden_info;
	ret->java.survivor_info = bmic_java_mem_survivor_info;
	ret->java.tenured_info = bmic_java_mem_tenured_info;
	ret->java.code_cache_info = bmic_java_mem_code_cache_info;
	ret->java.metaspace_info = bmic_java_mem_metaspace_info;
	ret->java.permgen_info = bmic_java_mem_permgen_info;

	return ret;
}

const char *bmic_activemq_base_url(const bmic_discovery_hint_t *hint) {
	if (activemq_base_url == NULL) {
		activemq_base_url = bmic_discovery_hint_to_url(
			hint, ACTIVEMQ_BASE_URL_HINT_URL, ACTIVEMQ_BASE_URL_HINT_ADDRESSING, 8161);
	}

	return activemq_base_url;
}

bmic_handle_t *bmic_activemq_init(
	const char *base_url, bmic_credentials_t *credentials, gru_status_t *status) {
	bmic_handle_t *handle = NULL;

	handle = gru_alloc(sizeof(bmic_handle_t), status);
	gru_alloc_check(handle, NULL);

	handle->ep = bmic_endpoint_init(base_url, NULL, NULL, status);
	if (handle->ep == NULL) {
		bmic_activemq_cleanup(&handle);

		return NULL;
	}

	bmic_endpoint_set_credentials(handle->ep, credentials, status);
	bmic_endpoint_http_begin(handle->ep, status);

	handle->transport.read = bmic_endpoint_http_read;
	handle->transport.write = bmic_endpoint_http_write;
	handle->path_formatter = bmic_path_formatter;

	return handle;
}

void bmic_activemq_cleanup(bmic_handle_t **handle) {
	bmic_handle_destroy(handle, bmic_endpoint_http_terminate);

	if (activemq_base_url) {
		gru_dealloc_const_string(&activemq_base_url);
	}

	bmic_exchange_destroy((bmic_exchange_t **) &cap_cache);
}

const bmic_exchange_t *bmic_activemq_load_capabilities(
	bmic_handle_t *handle, gru_status_t *status) {
	if (cap_cache) {
		return cap_cache;
	}

	bmic_exchange_t *ret = gru_alloc(sizeof(bmic_exchange_t), status);
	gru_alloc_check(ret, NULL);

	bmic_data_t reply = {0};
	bmic_api_io_read(handle, ACTIVEMQ_PRODUCT_CAPABILITIES, &reply, status);

	if (status->code != GRU_SUCCESS) {
		goto err_exit;
	}

	bmic_object_t *root = bmic_jolokia_parse(reply.data, status);
	bmic_data_release(&reply);
	if (!root) {
		goto err_exit;
	}

	const bmic_object_t *capabilities =
		bmic_object_find_regex(root, ACTIVEMQ_CAPABILITIES_KEY_REGEX, REG_SEARCH_NAME);
	if (!capabilities) {
		gru_status_set(status, GRU_FAILURE, "Capabilities not found");

		bmic_object_destroy(&root);
		goto err_exit;
	}

	ret->root = root;
	ret->data_ptr = capabilities;
	ret->type = EX_CAP_LIST;

	cap_cache = ret;

	return ret;

err_exit:
	bmic_data_release(&reply);
	gru_dealloc((void **) &ret);
	return NULL;
}

const bmic_exchange_t *bmic_activemq_attribute_read(bmic_handle_t *handle,
	const bmic_exchange_t *cap, const char *name, gru_status_t *status) {
	return bmic_activemq_mi_read(handle, cap->root, name, status, REG_SEARCH_NAME,
		ACTIVEMQ_CAPABILITIES_KEY_REGEX);
}

bmic_product_info_t *bmic_activemq_product_info(
	bmic_handle_t *handle, const bmic_exchange_t *cap, gru_status_t *status) {
	const bmic_exchange_t *ex = bmic_activemq_mi_read(handle, cap->root, "BrokerVersion",
		status, REG_SEARCH_NAME, ACTIVEMQ_CAPABILITIES_KEY_REGEX);

	if (ex == NULL) {
		return NULL;
	}

	if (status->code != GRU_SUCCESS) {
		bmic_exchange_destroy((bmic_exchange_t **) &ex);

		return NULL;
	}

	if (ex->data_ptr->type == BMIC_STRING) {
		bmic_product_info_t *ret = gru_alloc(sizeof(bmic_api_interface_t), status);
		strlcpy(ret->version, ex->data_ptr->data.str, sizeof(ret->version));
		strlcpy(ret->name, ACTIVEMQ_PRODUCT_NAME_PRETTY, sizeof(ret->name));

		bmic_exchange_destroy_const(&ex);
		return ret;
	}

	return NULL;
}

const bmic_list_t *bmic_activemq_attribute_list(
	bmic_handle_t *handle, const bmic_exchange_t *cap, gru_status_t *status) {
	const bmic_object_t *attributes = bmic_object_find_regex(
		cap->data_ptr, ACTIVEMQ_CORE_CAP_ATTRIBUTES, REG_SEARCH_PATH);
	if (attributes == NULL) {
		gru_status_set(status, GRU_FAILURE, "Attribute not found");

		return NULL;
	}

	bmic_list_t *ret = bmic_list_new(status, bmic_cap_info_destroy_list);
	gru_alloc_check(ret, NULL);

	bmic_payload_add_attr_t payload = {
		.list = ret->items, .status = status,
	};

	bmic_object_for_each(attributes, bmic_jolokia_translate_attr_object, &payload);

	return ret;
}

const bmic_exchange_t *bmic_activemq_queue_attribute_read(bmic_handle_t *handle,
	const bmic_exchange_t *capabilities, const char *name, gru_status_t *status,
	const char *queue) {
	return bmic_activemq_mi_read(handle, capabilities->root, name, status,
		REG_SEARCH_NAME, ACTIVEMQ_QUEUE_CAPABILITES_REGEX, queue);
}

const bmic_list_t *bmic_activemq_operation_list(
	bmic_handle_t *handle, const bmic_exchange_t *cap, gru_status_t *status) {
	const bmic_object_t *attributes =
		bmic_object_find_regex(cap->root, ACTIVEMQ_CAP_OPERATIONS, REG_SEARCH_PATH);
	if (attributes == NULL) {
		gru_status_set(status, GRU_FAILURE, "Attribute not found");

		return NULL;
	}

	bmic_list_t *ret = bmic_list_new(status, bmic_op_info_destroy_list);
	gru_alloc_check(ret, NULL);

	bmic_payload_add_attr_t payload = {
		.list = ret->items, .status = status,
	};

	bmic_object_for_each_child(attributes, bmic_jolokia_translate_op_object, &payload);

	return ret;
}

bool bmic_activemq_queue_create(bmic_handle_t *handle, const bmic_exchange_t *cap,
	const char *name, gru_status_t *status) {
	const bmic_object_t *attributes = bmic_object_find_regex(
		cap->root, ACTIVEMQ_BROKER_OPERATIONS_ROOT, REG_SEARCH_NAME);

	if (attributes == NULL) {
		gru_status_set(status, GRU_FAILURE, "Attribute not found");

		return false;
	}

	bmic_json_t *json = bmic_json_new(status);
	gru_alloc_check(json, false);

	bmic_activemq_json_create_queue(attributes, json, name);
	bool ret = bmic_jolokia_io_exec(handle, json, status);
	bmic_json_destroy(&json);

	return ret;
}

bool bmic_activemq_queue_delete(bmic_handle_t *handle, const bmic_exchange_t *cap,
	const char *name, gru_status_t *status) {
	const bmic_object_t *attributes = bmic_object_find_regex(
		cap->root, ACTIVEMQ_BROKER_OPERATIONS_ROOT, REG_SEARCH_NAME);
	if (attributes == NULL) {
		gru_status_set(status, GRU_FAILURE, "Attribute not found");

		return false;
	}

	bmic_json_t *json = bmic_json_new(status);
	gru_alloc_check(json, false);

	bmic_activemq_json_delete_queue(attributes, json, name);
	bool ret = bmic_jolokia_io_exec(handle, json, status);
	bmic_json_destroy(&json);

	return ret;
}

static const char *bmic_activemq_queue_filter_name(
	const char *fqdn, gru_status_t *status) {
	logger_t logger = gru_logger_get();
	const char *regex = "destinationName=([^,]+)";

	const char *ret = bmic_regex_find(fqdn, regex, 2, 1, status);
	if (!ret) {
		logger(ERROR, "Unable to parse the queue data for %s", fqdn);

		return NULL;
	}

	logger(DEBUG, "Found queue name %s for %s", ret, fqdn);

	return ret;
}

static void bmic_activemq_translate_queue_list(const void *nodedata, void *payload) {
	const bmic_object_t *nodeobj = (bmic_object_t *) nodedata;
	bmic_payload_add_attr_t *pl = (bmic_payload_add_attr_t *) payload;
	logger_t logger = gru_logger_get();

	logger(INFO, "Processing node %s [%s]", nodeobj->name, nodeobj->path);

	if (nodeobj->type == BMIC_OBJECT) {
		const bmic_object_t *obj = bmic_object_find_by_name(nodeobj, "objectName");

		if (obj && obj->type == BMIC_STRING) {
			const char *queue_name =
				bmic_activemq_queue_filter_name(obj->data.str, pl->status);

			logger(INFO, "Processing node %s [%s]: %s", nodeobj->name, nodeobj->path,
				queue_name);
			gru_list_append(pl->list, queue_name);
		} else {
			if (obj) {
				logger(WARNING, "Invalid sub node type for %s: %d", obj->name, obj->type);
			}
		}
	} else {
		logger(WARNING, "Invalid node type for %s: %d", nodeobj->name, nodeobj->type);
	}
}

const bmic_list_t *bmic_activemq_queue_list(
	bmic_handle_t *handle, const bmic_exchange_t *cap, gru_status_t *status) {
	const bmic_exchange_t *attributes =
		bmic_activemq_mi_read(handle, cap->root, ACTIVEMQ_QUEUE_LIST_ATTR, status,
			REG_SEARCH_NAME, ACTIVEMQ_CAPABILITIES_KEY_REGEX);

	if (attributes == NULL) {
		return NULL;
	}

	if (status->code != GRU_SUCCESS) {
		bmic_exchange_destroy((bmic_exchange_t **) &attributes);

		return NULL;
	}

	bmic_list_t *ret = bmic_list_new(status, gru_dealloc);
	gru_alloc_check(ret, NULL);

	bmic_payload_add_attr_t payload = {
		.list = ret->items, .status = status,
	};

	bmic_object_for_each_child(
		attributes->data_ptr, bmic_activemq_translate_queue_list, &payload);

	bmic_exchange_destroy((bmic_exchange_t **) &attributes);
	return ret;
}

static int64_t bmic_activemq_queue_stat_reader(bmic_handle_t *handle,
	const bmic_exchange_t *cap, const char *queue, const char *attr_name,
	gru_status_t *status) {

	const bmic_exchange_t *attribute =
		bmic_activemq_queue_attribute_read(handle, cap, attr_name, status, queue);

	logger_t logger = gru_logger_get();
	if (!attribute) {
		logger(ERROR, "Unavailable response for '%s' queue property", attr_name);
		if (status->code == GRU_SUCCESS) {
			gru_status_set(status, GRU_FAILURE,
				"Unavailable response for '%s' queue property", attr_name);
		}

		return -1;
	}

	int64_t ret = -1;
	if (attribute->data_ptr && attribute->data_ptr->type == BMIC_INTEGER) {
		ret = attribute->data_ptr->data.number;
	} else {
		logger(ERROR, "Invalid data pointer for the '%s' property", attr_name);

		gru_status_set(
			status, GRU_FAILURE, "Invalid data pointer for the '%s' property", attr_name);
	}

	bmic_exchange_destroy((bmic_exchange_t **) &attribute);
	return ret;
}

/**
 * List queues
 * @param handle
 * @param cap
 * @param status
 * @return
 */
bmic_queue_stat_t bmic_activemq_queue_stats(bmic_handle_t *handle,
	const bmic_exchange_t *cap, const char *queue, gru_status_t *status) {
	bmic_queue_stat_t ret = {0};
	ret.queue_size = bmic_activemq_queue_stat_reader(
		handle, cap, queue, ACTIVEMQ_QUEUE_SIZE_ATTR, status);
	if (status->code != GRU_SUCCESS) {
		return ret;
	}

	ret.msg_ack_count = bmic_activemq_queue_stat_reader(
		handle, cap, queue, ACTIVEMQ_QUEUE_ACK_CNT_ATTR, status);
	if (status->code != GRU_SUCCESS) {
		return ret;
	}

	ret.msg_exp_count = bmic_activemq_queue_stat_reader(
		handle, cap, queue, ACTIVEMQ_QUEUE_EXP_CNT_ATTR, status);
	if (status->code != GRU_SUCCESS) {
		return ret;
	}

	ret.consumer_count = bmic_activemq_queue_stat_reader(
		handle, cap, queue, ACTIVEMQ_QUEUE_CNS_CNT_ATTR, status);
	if (status->code != GRU_SUCCESS) {
		return ret;
	}

	return ret;
}