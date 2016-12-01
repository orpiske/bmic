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