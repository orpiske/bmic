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
    bmic_api_interface_t *ret = bmic_api_interface_new(ARTEMIS_PRODUCT_NAME,
                                                       ARTEMIS_API_VERSION, status);

    ret->base_url = bmic_artemis_base_url;
    ret->api_init = bmic_artemis_init;
    ret->product_info = bmic_artemis_product_info;
    ret->api_cleanup = bmic_artemis_cleanup;
    ret->capabilities_load = bmic_artemis_load_capabilities;
    ret->attribute_read = bmic_artemis_attribute_read;
    ret->attribute_list = bmic_artemis_attribute_list;
    ret->queue_attribute_read = bmic_artemis_queue_attribute_read;
    ret->operation_list = bmic_artemis_operation_list;
    ret->queue_create = bmic_artemis_queue_create;
    ret->queue_delete = bmic_artemis_queue_delete;
    ret->queue_list = bmic_artemis_queue_list;
    ret->queue_stats = bmic_artemis_queue_stats;

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

const char *bmic_artemis_base_url(const bmic_discovery_hint_t *hint)
{
    return bmic_discovery_hint_to_url(hint, ARTEMIS_BASE_URL_HINT_URL, 
                                      ARTEMIS_BASE_URL_HINT_ADDRESSING, 8161);
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
                                                    const bmic_exchange_t *cap,
                                                    gru_status_t *status)
{
    const bmic_exchange_t *ex = bmic_artemis_mi_read(handle, cap->root, "Version", status,
                                 REG_SEARCH_NAME, ARTEMIS_CAPABILITIES_KEY_REGEX);
    
    if (ex->data_ptr->type == STRING) {
        bmic_product_info_t *ret = gru_alloc(sizeof (bmic_api_interface_t), status);
        strlcpy(ret->version, ex->data_ptr->data.str, sizeof (ret->version));
        strlcpy(ret->name, ARTEMIS_PRODUCT_NAME_PRETTY, sizeof (ret->name));

        bmic_exchange_destroy(&ex);
        return ret;
    }
    
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

static void bmic_artemis_translate_queue_list(const void *nodedata, void *payload)
{
    const bmic_object_t *nodeobj = (bmic_object_t *) nodedata;
    bmic_payload_add_attr_t *pl =
            (bmic_payload_add_attr_t *) payload;
    logger_t logger = gru_logger_get();

    logger(INFO, "Processing node %s [%s]", nodeobj->name, nodeobj->path);

    if (nodeobj->type == STRING) {
        gru_list_append(pl->list, strdup(nodeobj->data.str));
    }
    else {
        logger(WARNING, "Invalid node type for %s: %d", nodeobj->name, nodeobj->type);
    }
}

const bmic_list_t *bmic_artemis_queue_list(bmic_handle_t *handle,
                                           const bmic_exchange_t *cap,
                                           gru_status_t *status)
{
    const bmic_exchange_t *attributes = bmic_artemis_mi_read(handle, cap->root,
                                                             ARTEMIS_QUEUE_LIST_ATTR,
                                                             status,
                                                             REG_SEARCH_NAME, ARTEMIS_CAPABILITIES_KEY_REGEX);

    bmic_list_t *ret = bmic_list_new(status, gru_dealloc);
    gru_alloc_check(ret, NULL);

    bmic_payload_add_attr_t payload = {
        .list = ret->items,
        .status = status,
    };


    bmic_object_for_each_child(attributes->data_ptr, bmic_artemis_translate_queue_list, &payload);
    
    bmic_exchange_destroy((bmic_exchange_t **) &attributes);

    return ret;
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

bool bmic_artemis_queue_create(bmic_handle_t *handle,
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

bool bmic_artemis_queue_delete(bmic_handle_t *handle,
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

/**
 * List queues
 * @param handle
 * @param cap
 * @param status
 * @return 
 */
bmic_queue_stat_t bmic_artemis_queue_stats(bmic_handle_t *handle,
                                          const bmic_exchange_t *cap,
                                          const char *queue,
                                          gru_status_t *status)
{
    bmic_queue_stat_t ret = {0};
    const bmic_exchange_t *qsize = bmic_artemis_queue_attribute_read(handle, cap,
                                                                     ARTEMIS_QUEUE_SIZE_ATTR,
                                                                     status, queue);
    logger_t logger = gru_logger_get();
    if (qsize) {
        if (qsize->data_ptr && qsize->data_ptr->type == INTEGER) {
            ret.queue_size = qsize->data_ptr->data.number;
        }
        else {
            logger(ERROR, "Invalid data pointer for the queue size property");
        }
        
        bmic_exchange_destroy((bmic_exchange_t **) &qsize);
    }
    else {
        logger(ERROR, "Unavailable response for queue size property");
    }

    const bmic_exchange_t *ack = bmic_artemis_queue_attribute_read(handle, cap,
                                                                   ARTEMIS_QUEUE_ACK_CNT_ATTR,
                                                                   status, queue);
    if (ack) {
        if (ack->data_ptr && ack->data_ptr->type == INTEGER) {
            ret.msg_ack_count = ack->data_ptr->data.number;
        }
        else {
            logger(ERROR, "Invalid data pointer for the acknowledge message count property");
        }
        
        bmic_exchange_destroy((bmic_exchange_t **) &ack);
    }
    else {
        logger(ERROR, "Unavailable response for acknowledge message count property");
    }

    const bmic_exchange_t *exp = bmic_artemis_queue_attribute_read(handle, cap,
                                                                   ARTEMIS_QUEUE_EXP_CNT_ATTR,
                                                                   status, queue);
    if (exp) {
        if (exp->data_ptr && exp->data_ptr->type == INTEGER) {
            ret.msg_exp_count = exp->data_ptr->data.number;
        }
        else {
            logger(ERROR, "Invalid data pointer for the expired message count property");
        }
        
        bmic_exchange_destroy((bmic_exchange_t **) &exp);
    }
    else {
        logger(ERROR, "Unavailable response for expired message count property");
    }

    const bmic_exchange_t *cns = bmic_artemis_queue_attribute_read(handle, cap,
                                                                   ARTEMIS_QUEUE_CNS_CNT_ATTR,
                                                                   status, queue);
    if (cns) {
        if (cns->data_ptr && cns->data_ptr->type == INTEGER) {
            ret.consumer_count = cns->data_ptr->data.number;
        }
        else {
            logger(ERROR, "Invalid data pointer for the consumers count property");
        }
        
        bmic_exchange_destroy((bmic_exchange_t **) &cns);
    }
    else {
        logger(ERROR, "Unavailable response for consumers count property");
    }

    
    
    return ret;
}