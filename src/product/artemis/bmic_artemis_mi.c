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

static const bmic_object_t *bmic_artemis_mi_read_from(bmic_handle_t *handle,
                                                       const char *cap_name,
                                                       const char *attr_name,
                                                       gru_status_t *status)
{
    bmic_data_t reply = {0};

    const char *path = handle->path_formatter(ARTEMIS_READ, cap_name,
                                              ARTMIS_BASE_PKG, attr_name, status);

    bmic_api_io_read(handle, path, &reply, status);
    gru_dealloc_string((char **) &path);

    if (status->code != GRU_SUCCESS) {
        return NULL;
    }

    return bmic_api_parse_json(reply.data, status);
}

static const bmic_cap_info_t *bmic_artermis_mi_read_attr_info(const bmic_object_t *capabilities, const char *attr_name,
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
    bmic_artemis_mi_translate_attr(value_attributes, info);
    return info;
}

const bmic_exchange_t *bmic_artemis_mi_read(bmic_handle_t *handle,
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

    bmic_cap_info_t *info = bmic_artermis_mi_read_attr_info(capabilities, 
                                                               attr_name, status);

    ////////////////////////////    
    /*
     * Uses the resolved capability (only uses the name, actually) to read the 
     * data from the BMIC.
     */
    bmic_object_t *reply_obj = bmic_artemis_mi_read_from(handle, capabilities->name,
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
    gru_dealloc((void **)info);
    bmic_object_destroy(&reply_obj);
    return NULL;
}


void bmic_artemis_mi_translate_attr(const bmic_object_t *obj,
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