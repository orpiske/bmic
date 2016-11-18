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
#include "bmic_op_info.h"


bmic_op_info_t *bmic_op_info_new(gru_status_t *status) {
    bmic_op_info_t *ret = gru_alloc(sizeof(bmic_op_info_t), status);
    gru_alloc_check(ret, NULL);
    
    ret->args = gru_list_new(status);
    if (!ret->args) {
        gru_dealloc((void **)&ret);
        
        return NULL;
    }

    return ret;
}

void bmic_op_info_destroy(bmic_op_info_t **ptr) {
    bmic_op_info_t *obj = *ptr;

    if (obj == NULL) {
        return;
    }

    if (obj->name != NULL) {
        gru_dealloc_string(&obj->name);
    }

    if (obj->description != NULL) {
        gru_dealloc_string(&obj->description);
    }

    if (obj->ret != NULL) {
        gru_dealloc_string(&obj->ret);
    }
    
    gru_list_destroy(&obj->args);

    gru_dealloc((void **) ptr);
}

void bmic_op_info_set_name(bmic_op_info_t *op, const char *name) {
    assert(op != NULL);

    if (asprintf(&op->name, "%s", name) == -1) {
        fprintf(stderr, "Unable to allocate memory for saving the operation name\n");
    }
}
void bmic_op_info_set_description(bmic_op_info_t *op, const char *description) {
    assert(op != NULL);

    if (asprintf(&op->description, "%s", description) == -1) {
        fprintf(stderr, "Unable to allocate memory for saving the operation description\n");
    }
}


void bmic_op_info_set_ret(bmic_op_info_t *op, const char *ret) {
    assert(op != NULL);

    if (asprintf(&op->ret, "%s", ret) == -1) {
        fprintf(stderr, "Unable to allocate memory for saving the operation return type\n");
    }
}


void bmic_op_info_add_arg(bmic_op_info_t *op, const bmic_op_arg_t *arg) {
    assert(op != NULL);
    assert(op->args != NULL);
    
    
    if (gru_list_append(op->args, arg) == NULL) {
        fprintf(stderr, "Unable to add new argument\n");
    }
}