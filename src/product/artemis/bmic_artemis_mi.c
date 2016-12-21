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
#include "bmic_artemis_mi.h"

const bmic_exchange_t *bmic_artemis_mi_read(bmic_handle_t *handle,
	const bmic_object_t *root, const char *attr_name, gru_status_t *status, int flags,
	const char *regex_fmt, ...) {

	bmic_exchange_t *ret = gru_alloc(sizeof(bmic_exchange_t), status);
	gru_alloc_check(ret, NULL);

	va_list ap;
	va_start(ap, regex_fmt);

	/*
	 *  First, resolve the root of the capability (and whether it even exist ...
	 *
	 * ie.: read the previous returned capability tree and finds the pointer
	 * to the node respective to the matching parameters (regex + flag)
	 */
	const bmic_object_t *capabilities =
		bmic_finder_varg(root, regex_fmt, flags, status, ap);
	va_end(ap);

	if (!capabilities) {
		gru_dealloc((void **) &ret);

		return NULL;
	}

	const bmic_cap_info_t *info =
		bmic_jolokia_read_attr_info(capabilities, attr_name, status);

	////////////////////////////
	/*
	 * Uses the resolved capability (only uses the name, actually) to read the
	 * data from the BMIC.
	 */
	const bmic_object_t *reply_obj = bmic_jolokia_io_read_attribute(
		handle, ARTEMIS_BASE_PKG, capabilities->name, attr_name, status);

	if (!reply_obj) {
		goto err_exit;
	}

	/*
	 * Gets the value children of the response object to transform it
	 */
	const bmic_object_t *value = bmic_object_find_by_name(reply_obj, "value");

	if (!value) {
		goto err_exit;
	}
	////////////////////////////

	/*
	 * Assign stuff to the exchange object
	 */
	ret->root = reply_obj;
	ret->data_ptr = value;
	ret->type = EX_CAP_ENTRY;
	ret->payload.capinfo = info;

	return ret;

err_exit:
	gru_dealloc((void **) &ret);
	gru_dealloc((void **) &info);
	bmic_object_destroy((bmic_object_t **) &reply_obj);
	return NULL;
}
