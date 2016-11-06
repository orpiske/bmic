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

bmic_api_interface_t *bmic_activemq_product(gru_status_t *status) {
    bmic_api_interface_t *ret = bmic_api_interface_new("activemq", "5.x.x", status);

    ret->base_url = bmic_activemq_base_url;
    ret->api_init = bmic_activemq_init;
    ret->api_cleanup = bmic_activemq_cleanup;
    ret->product_info = bmic_activemq_product_info;
    ret->capabilities = bmic_activemq_product_capabilities;
    ret->cap_read = bmic_activemq_product_cap_read;

    return ret;
}

const char *bmic_activemq_base_url(const bmic_discovery_hint_t *hint) {
    char *ret = NULL;

    if (hint->hint_type == URL) {
        asprintf(&ret, ACTIVEMQ_BASE_URL_HINT_URL,
                 hint->content.url);
    }
    else {
        asprintf(&ret, ACTIVEMQ_BASE_URL_HINT_ADDRESSING,
                 hint->content.addressing.hostname, 8161);
    }

    return ret;
}

bmic_handle_t *bmic_activemq_init(const char *base_url,
                                 bmic_credentials_t *credentials,
                                 gru_status_t *status) {
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

    return handle;
}

void bmic_activemq_cleanup(bmic_handle_t **handle) {
    bmic_handle_destroy(handle, bmic_endpoint_http_terminate);
}

bmic_product_info_t *bmic_activemq_product_info(bmic_handle_t *handle,
        gru_status_t *status)
{
    bmic_data_t reply = {0};
    bmic_api_io_read(handle, ACTIVEMQ_PRODUCT_INFO_PATH, &reply, status);

    if (status->code != GRU_SUCCESS) {
        return NULL;
    }
   
    bmic_object_t *root = bmic_api_parse_json(reply.data, status);
    if (!root) {
        return NULL;
    }

    const bmic_object_t *value = bmic_object_find_by_name(root, "value");
    if (!value) {
        goto err_exit;
    }

    if (value->type == STRING) {
        bmic_product_info_t *ret = gru_alloc(sizeof(bmic_api_interface_t), status);
        snprintf(ret->version, sizeof(ret->version), "%s", value->data.str);

        bmic_object_destroy(&root);
        return ret;
    }

    err_exit:
    bmic_object_destroy(&root);
    return NULL;
}




static bool bmic_activemq_management_path_compare(const void *nodedata,
                                                 const void *data, void *r)
{
    const bmic_object_t *nodeobj = (bmic_object_t *) nodedata;

    if (nodedata == NULL) {
        return false;
    }

    // TODO: possibly evaluate using regex instead.
    
    gru_status_t status = {0};
    bool match = bmic_match(nodeobj->name, 
                            "(brokerName=)([a-z0-9\\.])*(,type=Broker)", &status);
    
    // TODO: improve the error handling here
    if (status.code == GRU_FAILURE) {
        fprintf(stderr, "%s\n", status.message);
    }
    
    if (match == true || strcmp(nodeobj->name, "brokerName=localhost,type=Broker") == 0) {
        printf("Done");
    }
    
    return match;
}

const bmic_product_cap_t *bmic_activemq_product_capabilities(bmic_handle_t *handle,
                                                            gru_status_t *status)
{
    bmic_product_cap_t *ret = gru_alloc(sizeof (bmic_product_cap_t), status);
    gru_alloc_check(ret, NULL);

    bmic_data_t reply = {0};
    bmic_api_io_read(handle, ACTIVEMQ_PRODUCT_CAPABILITIES, &reply, status);

    if (status->code != GRU_SUCCESS) {
        gru_dealloc((void **)&ret);
        return NULL;
    }

    
    bmic_object_t *root = bmic_api_parse_json(reply.data, status);
    if (!root) {
        gru_dealloc((void **)&ret);
        return NULL;
    }

    const bmic_object_t *capabilities = bmic_object_find(root,
                                         bmic_activemq_management_path_compare,
                                         NULL);
    if (!capabilities) {
        gru_status_set(status, GRU_FAILURE, "Capabilities not found");
        
        bmic_object_destroy(&root);
        gru_dealloc((void **)&ret);
        
        return NULL;
    }
    
    ret->root = root;
    ret->capabilities = capabilities;
    
    return ret;
}

const char *format_activemq_path(const char *op, const bmic_product_cap_t *cap, 
                        const char *capname, gru_status_t *status)
{
    char *ret; 
    
    int rc = asprintf(&ret, "%s/org.apache.activemq:%s/%s", 
             op, cap->capabilities->name, capname);
    
    if (rc == -1) {
        gru_status_set(status, GRU_FAILURE, "Not enough memory to format capabilities path");
        
        return NULL;
    }
    
    return ret;
}


const bmic_object_t *bmic_activemq_product_cap_read(bmic_handle_t *handle,
        const bmic_product_cap_t *cap, const char *name,
        gru_status_t *status) 
{
    
    bmic_data_t reply = {0};
    
    const char *path = format_activemq_path("read", cap, name, status);
    if (!path) {
        return NULL;
    }
    
    bmic_api_io_read(handle, path, &reply, status);
    gru_dealloc_string((char **)&path);

    if (status->code != GRU_SUCCESS) {
        return NULL;
    }

    bmic_object_t *root = bmic_api_parse_json(reply.data, status);
    if (!root) {
        return NULL;
    }

    const bmic_object_t *value = bmic_object_find_by_name(root, "value");
    if (!value) {
        goto err_exit;
    }
    
    bmic_object_t *ret = bmic_object_clone(value, status); 
    bmic_object_destroy(&root);

    return ret;

err_exit:
    bmic_object_destroy(&root);
    return NULL;
}