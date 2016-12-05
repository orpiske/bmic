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
#include "bmic_artemis.h"

bmic_api_interface_t *bmic_artemis_product(gru_status_t *status)
{
    bmic_api_interface_t *ret = bmic_api_interface_new("artemis", "1.x.x", status);

    ret->base_url = bmic_artemis_base_url;
    ret->api_init = bmic_artemis_init;
    ret->product_info = bmic_artemis_product_info;
    ret->api_cleanup = bmic_artemis_cleanup;
    ret->load_capabilities = bmic_artemis_load_capabilities;
    ret->attribute_read = bmic_artemis_attribute_read;
    ret->attribute_list = bmic_artemis_attribute_list;
    ret->queue_attribute_read = bmic_artemis_queue_attribute_read;
    ret->operation_list = bmic_artemis_operation_list;
    ret->create_queue = bmic_artemis_operation_create_queue;
    ret->delete_queue = bmic_artemis_operation_delete_queue;
    // ret->java.java_info = bmic_java_read_info;
    
    /*
     * bmic_java_mem_info_t bmic_java_mem_eden_info(bmic_handle_t *handle, gru_status_t *status);
bmic_java_mem_info_t bmic_java_mem_survivor_info(bmic_handle_t *handle, gru_status_t *status);
bmic_java_mem_info_t bmic_java_mem_tenured_info(bmic_handle_t *handle, gru_status_t *status);
bmic_java_mem_info_t bmic_java_mem_code_cache_info(bmic_handle_t *handle, gru_status_t *status);
bmic_java_mem_info_t bmic_java_mem_metaspace_info(bmic_handle_t *handle, gru_status_t *status);
bmic_java_mem_info_t bmic_java_mem_permgen_info(bmic_handle_t *handle, gru_status_t *status);
     */
    ret->java.eden_info = bmic_java_mem_eden_info;
    ret->java.survivor_info = bmic_java_mem_survivor_info;
    ret->java.tenured_info = bmic_java_mem_tenured_info;
    ret->java.code_cache_info = bmic_java_mem_code_cache_info;
    ret->java.metaspace_info = bmic_java_mem_metaspace_info;
    ret->java.permgen_info = bmic_java_mem_permgen_info;
    
    return ret;
}

const char *bmic_artemis_base_url(const bmic_discovery_hint_t *hint)
{
    char *ret = NULL;

    if (hint->hint_type == URL) {
        if (asprintf(&ret, ARTEMIS_BASE_URL_HINT_URL, hint->content.url) == -1) {
            logger_t logger = gru_logger_get();
        
            logger(FATAL, "Not enough memory to set URL hint");
        
            return NULL;
        }
    }
    else {
        if (asprintf(&ret, ARTEMIS_BASE_URL_HINT_ADDRESSING, 
                     hint->content.addressing.hostname, 8161) == -1) {
            logger_t logger = gru_logger_get();
        
            logger(FATAL, "Not enough memory to set addressing hint");
        
            return NULL;
        }
    }

    return ret;
}

bmic_handle_t *bmic_artemis_init(const char *base_url,
                                 bmic_credentials_t *credentials,
                                 gru_status_t *status)
{
    bmic_handle_t *handle = NULL;

    handle = gru_alloc(sizeof (bmic_handle_t), status);
    gru_alloc_check(handle, NULL);

    handle->ep = bmic_endpoint_init(base_url, NULL, NULL, status);
    if (handle->ep == NULL) {
        bmic_artemis_cleanup(&handle);

        return NULL;
    }

    bmic_endpoint_set_credentials(handle->ep, credentials, status);
    bmic_endpoint_http_begin(handle->ep, status);

    handle->transport.read = bmic_endpoint_http_read;
    handle->transport.write = bmic_endpoint_http_write;
    handle->path_formatter = bmic_path_formatter;

    return handle;
}

void bmic_artemis_cleanup(bmic_handle_t **handle)
{
    bmic_handle_destroy(handle, bmic_endpoint_http_terminate);
}

bmic_product_info_t *bmic_artemis_product_info(bmic_handle_t *handle,
                                               gru_status_t *status)
{
    bmic_data_t reply = {0};
    bmic_api_io_read(handle, ARTEMIS_PRODUCT_INFO_PATH, &reply, status);

    if (status->code != GRU_SUCCESS) {
        bmic_data_release(&reply);
        return NULL;
    }

    bmic_object_t *root = bmic_jolokia_parse(reply.data, status);
    bmic_data_release(&reply);
    if (!root) {
        return NULL;
    }

    const bmic_object_t *value = bmic_object_find_by_name(root, "value");
    if (!value) {
        goto err_exit;
    }

    if (value->type == STRING) {
        bmic_product_info_t *ret = gru_alloc(sizeof (bmic_api_interface_t), status);
        snprintf(ret->version, sizeof (ret->version), "%s", value->data.str);

        bmic_object_destroy(&root);
        return ret;
    }

err_exit:
    bmic_object_destroy(&root);
    return NULL;
}

const bmic_exchange_t *bmic_artemis_load_capabilities(bmic_handle_t *handle,
                                                      gru_status_t *status)
{
    bmic_exchange_t *ret = gru_alloc(sizeof (bmic_exchange_t), status);
    gru_alloc_check(ret, NULL);

    bmic_data_t reply = {0};
    bmic_api_io_read(handle, ARTEMIS_PRODUCT_CAPABILITIES, &reply, status);
    
    
    if (status->code != GRU_SUCCESS) {
        goto err_exit;
    }


    bmic_object_t *root = bmic_jolokia_parse(reply.data, status);
    if (!root) {
        goto err_exit;
    }


    const bmic_object_t *capabilities = bmic_object_find_regex(root,
                                                               ARTEMIS_CAPABILITIES_KEY_REGEX,
                                                               REG_SEARCH_NAME);

    if (!capabilities) {
        gru_status_set(status, GRU_FAILURE, "Capabilities not found");
        bmic_object_destroy(&root);

        goto err_exit;
    }
    bmic_data_release(&reply);

    ret->root = root;
    ret->data_ptr = capabilities;
    ret->type = EX_CAP_LIST;

    return ret;

err_exit:
    bmic_data_release(&reply);
    gru_dealloc((void **) &ret);
    return NULL;
}


const bmic_exchange_t *bmic_artemis_attribute_read(bmic_handle_t *handle,
                                                   const bmic_exchange_t *cap,
                                                   const char *name,
                                                   gru_status_t *status)
{
    return bmic_artemis_mi_read(handle, cap->root, name, status,
                             REG_SEARCH_NAME, ARTEMIS_CAPABILITIES_KEY_REGEX);
}

const bmic_list_t *bmic_artemis_attribute_list(bmic_handle_t *handle,
                                              const bmic_exchange_t *cap, gru_status_t *status)
{
    const bmic_object_t *attributes = bmic_object_find_regex(cap->data_ptr,
                                                             ARTEMIS_CORE_CAP_ATTRIBUTES,
                                                             REG_SEARCH_PATH);
    bmic_list_t *ret = bmic_list_new(status, bmic_cap_info_destroy_list);
    gru_alloc_check(ret, NULL);

    bmic_payload_add_attr_t payload = {
        .list = ret->items,
        .status = status,
    };


    bmic_object_for_each(attributes, bmic_jolokia_translate_attr_object, &payload);

    return ret;
}

const bmic_exchange_t *bmic_artemis_queue_attribute_read(bmic_handle_t *handle,
                                                         const bmic_exchange_t *capabilities, const char *name,
                                                         gru_status_t *status, const char *queue)
{
    return bmic_artemis_mi_read(handle, capabilities->root, name, status,
                             REG_SEARCH_NAME, ARTEMIS_QUEUE_CAPABILITES_REGEX,
                             queue);
}

const bmic_list_t *bmic_artemis_operation_list(bmic_handle_t *handle,
                                              const bmic_exchange_t *cap, gru_status_t *status)
{
    const bmic_object_t *attributes = bmic_object_find_regex(cap->root,
                                                             ARTEMIS_CORE_CAP_OPERATIONS,
                                                             REG_SEARCH_PATH);
    bmic_list_t *ret = bmic_list_new(status, bmic_op_info_destroy_list);
    gru_alloc_check(ret, NULL);

    bmic_payload_add_attr_t payload = {
        .list = ret->items,
        .status = status,
    };


    bmic_object_for_each_child(attributes, bmic_jolokia_translate_op_object, &payload);

    return ret;
}


bool bmic_artemis_operation_create_queue(bmic_handle_t *handle,
                                            const bmic_exchange_t *cap, 
                                            const char *name,
                                            gru_status_t *status)
{
    const bmic_object_t *attributes = bmic_object_find_regex(cap->root,
                                                             ARTEMIS_CORE_BROKER_OPERATIONS_ROOT,
                                                             REG_SEARCH_NAME);
    
    bmic_json_t *json = bmic_json_new(status);
    gru_alloc_check(json, false);
    
    bmic_artemis_json_create_queue(attributes, json, name);
    bool ret = bmic_jolokia_io_exec(handle, json, status);
    bmic_json_destroy(&json);
    
    return ret;
}

bool bmic_artemis_operation_delete_queue(bmic_handle_t *handle,
                                            const bmic_exchange_t *cap, 
                                            const char *name,
                                            gru_status_t *status)
{
    const bmic_object_t *attributes = bmic_object_find_regex(cap->root,
                                                             ARTEMIS_CORE_BROKER_OPERATIONS_ROOT,
                                                             REG_SEARCH_NAME);
    
    bmic_json_t *json = bmic_json_new(status);
    gru_alloc_check(json, false);
    
    bmic_artemis_json_destroy_queue(attributes, json, name);
    bool ret = bmic_jolokia_io_exec(handle, json, status);
    bmic_json_destroy(&json);
    
    return ret;
}