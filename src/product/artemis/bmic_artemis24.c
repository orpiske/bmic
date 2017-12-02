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
#include "bmic_artemis24.h"
#include "product/artemis/bmic_artemis.h"

static const char *artemis24_base_url;

bmic_api_interface_t *bmic_artemis24_product(gru_status_t *status) {
	bmic_api_interface_t *ret =
			bmic_api_interface_new(ARTEMIS24_PRODUCT_NAME, ARTEMIS24_API_VERSION, status);

	ret->base_url = bmic_artemis24_base_url;
	ret->api_init = bmic_artemis_init;
	ret->product_info = bmic_artemis24_product_info;
	ret->api_cleanup = bmic_artemis24_cleanup;
	ret->capabilities_load = bmic_artemis_load_capabilities;
	ret->attribute_read = bmic_artemis_attribute_read;
	ret->attribute_list = bmic_artemis_attribute_list;
	ret->queue_attribute_read = bmic_artemis_queue_attribute_read;
	ret->operation_list = bmic_artemis_operation_list;
	ret->queue_create = bmic_artemis_queue_create;
	ret->queue_delete = bmic_artemis_queue_delete;
	ret->queue_list = bmic_artemis_queue_list;
	ret->queue_stats = bmic_artemis_queue_stats;
	ret->queue_purge = bmic_artemis_queue_purge;
	ret->queue_reset = bmic_artemis_queue_reset;

	// Artemis treats queues and topics, mostly, the same way and
	// topics are listed along w/ queues
	ret->topic_list = bmic_artemis_queue_list;
	ret->topic_stats = bmic_artemis_topic_stats;

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

const char *bmic_artemis24_base_url(const bmic_discovery_hint_t *hint) {
	if (artemis24_base_url == NULL) {
		artemis24_base_url = bmic_discovery_hint_to_url(
			hint, ARTEMIS24_BASE_URL_HINT_URL, ARTEMIS24_BASE_URL_HINT_ADDRESSING, 8161);
	}

	return artemis24_base_url;
}

void bmic_artemis24_cleanup(bmic_handle_t **handle) {
	bmic_handle_destroy(handle, bmic_endpoint_http_terminate);

	if (artemis24_base_url) {
		gru_dealloc_const_string(&artemis24_base_url);
	}

	bmic_artemis_cleanup(handle);
}

bmic_product_info_t *bmic_artemis24_product_info(
		bmic_handle_t *handle, const bmic_exchange_t *cap, gru_status_t *status) {
	const bmic_exchange_t *ex = bmic_artemis_mi_read(handle,
													 cap->root,
													 "Version",
													 status,
													 REG_SEARCH_NAME,
													 ARTEMIS_CAPABILITIES_KEY_V20_REGEX);

	if (ex == NULL) {
		return NULL;
	}

	if (gru_status_error(status)) {
		bmic_exchange_destroy((bmic_exchange_t **) &ex);

		return NULL;
	}

	if (ex->data_ptr->type == BMIC_STRING) {
		bmic_product_info_t *ret = gru_alloc(sizeof(bmic_product_info_t), status);
		strlcpy(ret->version, ex->data_ptr->data.str, sizeof(ret->version));
		strlcpy(ret->name, ARTEMIS_PRODUCT_NAME_PRETTY, sizeof(ret->name));

		bmic_exchange_destroy((bmic_exchange_t **) &ex);
		return ret;
	}

	return NULL;
}
