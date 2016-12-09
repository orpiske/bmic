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
#include "bmic_op_sig.h"


bmic_op_sig_t *bmic_op_sig_new(gru_status_t *status) {
    bmic_op_sig_t *ret = gru_alloc(sizeof(bmic_op_sig_t), status);
    gru_alloc_check(ret, NULL);
    
    ret->args = gru_list_new(status);
    if (!ret->args) {
        bmic_op_sig_destroy(&ret);
        
        return NULL;
    }
    
    return ret;
}

static void bmic_op_sig_destroy_args(const void *nodedata, void *payload) {
    bmic_op_arg_destroy((bmic_op_arg_t **)&nodedata);
}

void bmic_op_sig_destroy(bmic_op_sig_t **ptr) {
    bmic_op_sig_t *sig = *ptr;
    
    if (!sig) {
        return;
    }
    
    if (sig->args) {
        gru_list_for_each(sig->args, bmic_op_sig_destroy_args, NULL);
        gru_list_destroy(&sig->args);
    }
    
    if (sig->description != NULL) {
        gru_dealloc_string(&sig->description);
    }

    if (sig->ret != NULL) {
        gru_dealloc_string(&sig->ret);
    }
    
    gru_dealloc((void **) ptr);
}

void bmic_op_sig_add_arg(bmic_op_sig_t *sig, const bmic_op_arg_t *arg) {
    if (gru_list_append(sig->args, arg) == NULL) {
        logger_t logger = gru_logger_get();
        
        logger(FATAL, "Unable to add new argument");
    }
}


void bmic_op_sig_set_description(bmic_op_sig_t *sig, const char *description) {
    assert(sig != NULL);

    if (asprintf(&sig->description, "%s", description) == -1) {
        logger_t logger = gru_logger_get();
        
        logger(FATAL, "Unable to allocate memory for saving the signature description");
    }
}


void bmic_op_sig_set_ret(bmic_op_sig_t *sig, const char *ret) {
    assert(sig != NULL);

    if (asprintf(&sig->ret, "%s", ret) == -1) {
        logger_t logger = gru_logger_get();
        
        logger(FATAL, "Unable to allocate memory for saving the signature return type");
    }
}