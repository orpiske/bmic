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
#include "bmic_jamq6.h"
#include "product/artemis/bmic_artemis.h"

static const char *jamq6_base_url;

bmic_api_interface_t *bmic_jamq6_product(gru_status_t *status) {
	bmic_api_interface_t *ret =
		bmic_api_interface_new(JAMQ6_PRODUCT_NAME, JAMQ6_API_VERSION, status);

	ret->base_url = bmic_jamq6_base_url;
	ret->api_init = bmic_activemq_init;
	ret->api_cleanup = bmic_jamq6_cleanup;
	ret->product_info = bmic_jamq6_product_info;
	ret->capabilities_load = bmic_activemq_load_capabilities;
	ret->attribute_read = bmic_activemq_attribute_read;
	ret->attribute_list = bmic_activemq_attribute_list;
	ret->queue_attribute_read = bmic_activemq_queue_attribute_read;
	ret->operation_list = bmic_activemq_operation_list;
	ret->queue_create = bmic_activemq_queue_create;
	ret->queue_delete = bmic_activemq_queue_delete;
	ret->queue_list = bmic_activemq_queue_list;
	ret->queue_stats = bmic_activemq_queue_stats;
	ret->queue_purge = bmic_activemq_queue_purge;
	
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

const char *bmic_jamq6_base_url(const bmic_discovery_hint_t *hint) {
	if (jamq6_base_url == NULL) {
		jamq6_base_url = bmic_discovery_hint_to_url(
			hint, JAMQ6_BASE_URL_HINT_URL, JAMQ6_BASE_URL_HINT_ADDRESSING, 8181);
	}

	return jamq6_base_url;
}

void bmic_jamq6_cleanup(bmic_handle_t **handle) {
	if (jamq6_base_url) {
		gru_dealloc_const_string(&jamq6_base_url);
	}

	bmic_activemq_cleanup(handle);
}

bmic_product_info_t *bmic_jamq6_product_info(
	bmic_handle_t *handle, const bmic_exchange_t *cap, gru_status_t *status) {
	bmic_product_info_t *ret = bmic_activemq_product_info(handle, cap, status);
	if (ret) {
		strlcpy(ret->name, "JBoss A-MQ6", sizeof(ret->name));

		return ret;
	} else {
		const bmic_exchange_t *ex = bmic_activemq_mi_read(handle, cap->root, "BrokerName",
			status, REG_SEARCH_NAME, ACTIVEMQ_CAPABILITIES_KEY_REGEX);

		if (ex && ex->data_ptr->type == BMIC_STRING) {
			ret = gru_alloc(sizeof(bmic_api_interface_t), status);
			strlcpy(ret->version, "Unknown", sizeof(ret->version));
			strlcpy(ret->name, "JBoss A-MQ6", sizeof(ret->name));

			bmic_exchange_destroy((bmic_exchange_t **) &ex);
			return ret;
		}
	}

	return NULL;
}
