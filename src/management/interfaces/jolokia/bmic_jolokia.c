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
#include "bmic_jolokia.h"

void bmic_jolokia_translate_attr(const bmic_object_t *obj,
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

static void bmic_jolokia_translate_arg_value(const void *nodedata, void *payload) {
    const bmic_object_t *node = (const bmic_object_t *) nodedata;
    bmic_op_sig_t *sig = (bmic_op_sig_t *) payload;
    
    // TODO: fix this
    bmic_op_arg_t *arg = bmic_op_arg_new(NULL);
    
    const bmic_object_t *name = bmic_object_find_by_name(node, "name");
    if (name && name->type == STRING) {
        bmic_op_arg_set_name(arg, name->data.str);
    } 

    const bmic_object_t *type = bmic_object_find_by_name(node, "type");
    if (type && type->type == STRING) {
        bmic_op_arg_set_type(arg, type->data.str);
    }

    const bmic_object_t *desc = bmic_object_find_by_name(node, "desc");
    if (desc && desc->type == STRING) {
        bmic_op_arg_set_description(arg, desc->data.str);
    }
    
    bmic_op_sig_add_arg(sig, arg);
}

static void bmic_jolokia_translate_sigs(const void *nodedata, void *payload) {
    const bmic_object_t *node = (const bmic_object_t *) nodedata;
    bmic_op_info_t *info = (bmic_op_info_t *) payload;
    
    const bmic_object_t *args = bmic_object_find_by_name(node, "args");
    bmic_op_sig_t *sig = bmic_op_sig_new(NULL);
    
    if (args && args->type == LIST) {
        bmic_object_for_each_child(args, bmic_jolokia_translate_arg_value, 
                                   sig);
    }
    
    const bmic_object_t *ret = bmic_object_find_child_by_name(node, "ret");
    if (ret && ret->type == STRING) {
        bmic_op_sig_set_ret(sig, ret->data.str);
    }

    const bmic_object_t *desc = bmic_object_find_child_by_name(node, "desc");
    if (desc && desc->type == STRING) {
        bmic_op_sig_set_description(sig, desc->data.str);
    }
    
    bmic_op_info_add_signature(info, sig);
    
}

void bmic_jolokia_translate_op(const bmic_object_t *obj,
                                         bmic_op_info_t *info,
                                  gru_status_t *status)
{
    if (obj->type == LIST) {
        bmic_object_for_each_child(obj, bmic_jolokia_translate_sigs, info);
    }
    else {
        bmic_jolokia_translate_sigs(obj, info);
    }
}


bool bmic_jolokia_translate_status(bmic_object_t *root, gru_status_t *status) {
    bmic_object_t *response_status = bmic_object_find_child_by_name(root, "status");
    
    if (response_status && response_status->type == INTEGER) {
        if (response_status->data.number != JOLOKIA_STATUS_OK) {
            bmic_object_t *error = bmic_object_find_child_by_name(root, "error");
            if (error && error->type == STRING) {
                gru_status_set(status, GRU_FAILURE, "Error %d: %s", 
                               response_status->data.number, 
                               error->data.str);
            }
            else {
                gru_status_set(status, GRU_FAILURE, "Unknown error: %d", 
                               response_status->data.number);
            }
        }
        
        logger_t logger = gru_logger_get();
        
        logger(DEBUG, "Artemis response status: %d", response_status->data.number);
        return true;
    }
    
    return false;
        
}