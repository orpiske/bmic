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

    return ret;
}

const char *bmic_artemis_base_url(const bmic_discovery_hint_t *hint)
{
    char *ret = NULL;

    if (hint->hint_type == URL) {
        asprintf(&ret, ARTEMIS_BASE_URL_HINT_URL,
                 hint->content.url);
    }
    else {
        asprintf(&ret, ARTEMIS_BASE_URL_HINT_ADDRESSING,
                 hint->content.addressing.hostname, 8161);
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


    bmic_object_t *root = bmic_api_parse_json(reply.data, status);
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

    ret->root = root;
    ret->data_ptr = capabilities;
    ret->type = EX_CAP_LIST;

    return ret;

err_exit:
    gru_dealloc((void **) &ret);
    return NULL;
}

const char *bmic_artermis_cap_attr_path(const bmic_object_t *obj, const char *name,
                                        gru_status_t *status)
{
    char *ret;

    int rc = asprintf(&ret, "/value/%s/attr/%s", obj->name, name);

    if (rc == -1) {
        gru_status_set(status, GRU_FAILURE, "Not enough memory to format capabilities path");

        return NULL;
    }

    return ret;
}

/**
 * Given a node of attributes, read them into the info object
 * @param obj
 * @param info
 */
static void bmic_artemis_read_attributes(const bmic_object_t *obj,
                                         bmic_cap_info_t *info)
{
    const bmic_object_t *rw = bmic_object_find_by_name(obj, "rw");
    if (rw && rw->type == BOOLEAN) {
        bmic_cap_info_set_write(info, rw->data.value);
    }

    const bmic_object_t *type = bmic_object_find_by_name(obj, "type");
    if (type && type->type == STRING) {
        bmic_cap_info_set_typename(info, type->data.str);
    }

    const bmic_object_t *desc = bmic_object_find_by_name(obj, "desc");
    if (desc && desc->type == STRING) {
        bmic_cap_info_set_description(info, desc->data.str);
    }
}

const bmic_exchange_t *bmic_artemis_attribute_read(bmic_handle_t *handle,
                                                   const bmic_exchange_t *cap,
                                                   const char *name,
                                                   gru_status_t *status)
{

    bmic_exchange_t *ret = gru_alloc(sizeof (bmic_exchange_t), status);
    gru_alloc_check(ret, NULL);

    bmic_data_t reply = {0};
    const char *path = handle->path_formatter(ARTEMIS_READ, cap->data_ptr->name,
                                              ARTMIS_BASE_PKG, name, status);
    if (!path) {
        goto err_exit;
    }

    bmic_api_io_read(handle, path, &reply, status);
    gru_dealloc_string((char **) &path);

    if (status->code != GRU_SUCCESS) {
        goto err_exit;
    }

    bmic_object_t *root = bmic_api_parse_json(reply.data, status);
    if (!root) {
        goto err_exit;
    }

    const bmic_object_t *value = bmic_object_find_by_name(root, "value");

    if (!value) {
        bmic_object_destroy(&root);

        goto err_exit;
    }


    const char *rev = bmic_artermis_cap_attr_path(cap->data_ptr, name, status);

    const bmic_object_t *value_attributes = bmic_object_find_by_path(cap->data_ptr,
                                                                     rev);
    gru_dealloc_string((char **) &rev);

    bmic_cap_info_t *info = bmic_cap_info_new(status);
    if (!info) {
        goto err_exit;
    }

    bmic_cap_info_set_name(info, name);
    bmic_artemis_read_attributes(value_attributes, info);

    ret->root = root;
    ret->data_ptr = value;
    ret->type = EX_CAP_ENTRY;
    ret->payload.capinfo = info;

    return ret;

err_exit:
    bmic_object_destroy(&root);
    return NULL;
}

static void bmic_artemis_add_attr(const void *nodedata, void *payload)
{
    const bmic_object_t *nodeobj = (bmic_object_t *) nodedata;
    bmic_payload_add_attr_t *pl =
            (bmic_payload_add_attr_t *) payload;

    if (nodeobj->type == OBJECT) {
        if (nodeobj->name && strcmp(nodeobj->name, "attr") != 0) {
            bmic_cap_info_t *info = bmic_cap_info_new(pl->status);

            if (!info) {
                return;
            }

            bmic_cap_info_set_name(info, nodeobj->name);
            bmic_artemis_read_attributes(nodeobj, info);

            gru_list_append(pl->list, info);
        }
    }

}

const gru_list_t *bmic_artemis_attribute_list(bmic_handle_t *handle,
                                              const bmic_exchange_t *cap, gru_status_t *status)
{
    const bmic_object_t *attributes = bmic_object_find_regex(cap->data_ptr,
                                                             ARTEMIS_CORE_CAP_ATTRIBUTES,
                                                             REG_SEARCH_PATH);
    gru_list_t *ret = gru_list_new(status);
    gru_alloc_check(ret, NULL);

    bmic_payload_add_attr_t payload = {
        .list = ret,
        .status = status,
    };


    bmic_object_for_each(attributes, bmic_artemis_add_attr, &payload);

    return ret;
}
///////////////////////////

static const bmic_object_t *bmic_api_io_read_attribute(bmic_handle_t *handle,
                                                       const bmic_object_t *capabilities,
                                                       const char *attr_name,
                                                       gru_status_t *status)
{
    bmic_data_t reply = {0};

    const char *path = handle->path_formatter(ARTEMIS_READ, capabilities->name,
                                              ARTMIS_BASE_PKG, attr_name, status);

    bmic_api_io_read(handle, path, &reply, status);
    gru_dealloc_string((char **) &path);

    if (status->code != GRU_SUCCESS) {
        return NULL;
    }

    return bmic_api_parse_json(reply.data, status);
}

static const bmic_cap_info_t *bmic_artermis_read_attribute_metadata(
                                                                    const bmic_object_t *capabilities, const char *attr_name,
                                                                    gru_status_t *status)
{
    /*
     * Gets the path to the attribute???
     * 
     * ie.: /value/address=\"test.performance.queue\",brokerName=\"0.0.0.0\",module=Core,name=\"test.performance.queue\",serviceType=Queue,type=Broker/attr/MessageCount
     * 
     */
    const char *rev = bmic_artermis_cap_attr_path(capabilities, attr_name, status);
    if (!rev) {
        return NULL;
    }


    /*
     * Read the attributes of the value itself (ie.: whether it's rw, type, desc)
     */
    const bmic_object_t *value_attributes = bmic_object_find_by_path(capabilities,
                                                                     rev);
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
                       "Unable to allocate memory for the capability metadata",
                       attr_name);

        return NULL;
    }

    bmic_cap_info_set_name(info, attr_name);
    bmic_artemis_read_attributes(value_attributes, info);
    return info;
}

static const bmic_exchange_t *bmic_artemis_read(bmic_handle_t *handle,
                                                const bmic_object_t *root,
                                                const char *attr_name,
                                                gru_status_t *status,
                                                int flags,
                                                const char *regex_fmt,
                                                ...)
{

    bmic_exchange_t *ret = gru_alloc(sizeof (bmic_exchange_t), status);
    gru_alloc_check(ret, NULL);

    va_list ap;
    va_start(ap, regex_fmt);

    /*
     *  First, resolve the root of the capability (and whether it even exist ...
     * 
     * ie.: read the previous returned capability tree and finds the pointer 
     * to the node respective to the matching parameters (regex + flag)
     */
    const bmic_object_t *capabilities = bmic_finder_varg(root, regex_fmt,
                                                         flags,
                                                         status,
                                                         ap);
    va_end(ap);

    if (!capabilities) {
        return NULL;
    }

    bmic_cap_info_t *info = bmic_artermis_read_attribute_metadata(capabilities, 
                                                               attr_name, status);

    ////////////////////////////    
    /*
     * Uses the resolved capability (only uses the name, actually) to read the 
     * data from the BMIC.
     */
    bmic_object_t *reply_obj = bmic_api_io_read_attribute(handle, capabilities,
                                                          attr_name,
                                                          status);
    if (!reply_obj) {
        goto err_exit;
    }

    /*
     * Gets the value children of the response object to transform it 
     */
    const bmic_object_t *value = bmic_object_find_by_name(reply_obj, "value");

    if (!value) {
        // No REPLY?
        bmic_object_destroy(&reply_obj);

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
    bmic_object_destroy(&root);
    return NULL;
}

const bmic_exchange_t *bmic_artemis_queue_attribute_read(bmic_handle_t *handle,
                                                         const bmic_exchange_t *capabilities, const char *name,
                                                         gru_status_t *status, const char *queue)
{
    return bmic_artemis_read(handle, capabilities->root, name, status,
                             REG_SEARCH_NAME, ARTEMIS_QUEUE_CAPABILITES_REGEX,
                             queue);
}