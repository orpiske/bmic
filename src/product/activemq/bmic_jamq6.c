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


bmic_api_interface_t *bmic_jamq6_product(gru_status_t *status)
{
    bmic_api_interface_t *ret = bmic_api_interface_new(JAMQ6_PRODUCT_NAME,
                                                       JAMQ6_API_VERSION, status);

    ret->base_url = bmic_jamq6_base_url;
    ret->api_init = bmic_activemq_init;
    ret->api_cleanup = bmic_activemq_cleanup;
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

const char *bmic_jamq6_base_url(const bmic_discovery_hint_t *hint)
{
    char *ret = NULL;

    if (hint->hint_type == URL) {
        if (asprintf(&ret, JAMQ6_BASE_URL_HINT_URL, hint->content.url) == -1) {
            logger_t logger = gru_logger_get();

            logger(FATAL, "Not enough memory to set URL hint");

            return NULL;
        }
    }
    else {
        if (asprintf(&ret, JAMQ6_BASE_URL_HINT_ADDRESSING,
                     hint->content.addressing.hostname, 8181) == -1) {
            logger_t logger = gru_logger_get();

            logger(FATAL, "Not enough memory to set addressing hint");

            return NULL;
        }
    }

    return ret;
}


bmic_product_info_t *bmic_jamq6_product_info(bmic_handle_t *handle,
                                                gru_status_t *status)
{
    bmic_data_t reply = {0};
    bmic_api_io_read(handle, ACTIVEMQ_PRODUCT_INFO_PATH, &reply, status);

    if (status->code != GRU_SUCCESS) {
        bmic_data_release(&reply);
        return NULL;
    }

    bmic_object_t *root = bmic_jolokia_parse(reply.data, status);
    bmic_data_release(&reply);
    if (!root) {
        return NULL;
    }

    
    logger_t logger = gru_logger_get();
    const bmic_object_t *value = bmic_object_find_by_name(root, JOLOKIA_OBJ_VALUE_NAME);
    if (!value || value->type != STRING) {
        /* 
         * So, jamq6 usually prevents querying the broker version by default ... but it 
         * does reply with a deny, so that is kind of our hint that it's jamq6
         */
        logger(ERROR, "Jolokia value object not found for jamq6");
        
        bmic_product_info_t *ret = gru_alloc(sizeof (bmic_api_interface_t), status);
        snprintf(ret->version, sizeof (ret->version), "6.x.x (unknown)");

        bmic_object_destroy(&root);
        return ret;    
    }
    
    if (value->type == STRING) {
        bmic_product_info_t *ret = gru_alloc(sizeof (bmic_api_interface_t), status);
        snprintf(ret->version, sizeof (ret->version), "%s", value->data.str);

        bmic_object_destroy(&root);
        return ret;
    }

    return NULL;
}