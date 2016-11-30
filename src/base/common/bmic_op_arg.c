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
#include "bmic_op_arg.h"



bmic_op_arg_t *bmic_op_arg_new(gru_status_t *status) {
    bmic_op_arg_t *ret = gru_alloc(sizeof(bmic_op_arg_t), status);
    gru_alloc_check(ret, NULL);

    return ret;
}

void bmic_op_arg_destroy(bmic_op_arg_t **ptr) {
    bmic_op_arg_t *obj = *ptr;

    if (obj == NULL) {
        return;
    }

    if (obj->name != NULL) {
        gru_dealloc_string(&obj->name);
    }

    if (obj->description != NULL) {
        gru_dealloc_string(&obj->description);
    }

    if (obj->type!= NULL) {
        gru_dealloc_string(&obj->type);
    }

    gru_dealloc((void **) ptr);
}

void bmic_op_arg_set_name(bmic_op_arg_t *arg, const char *name) {
    assert(arg != NULL);

    if (asprintf(&arg->name, "%s", name) == -1) {
        logger_t logger = gru_logger_get();
        logger(FATAL, "Unable to allocate memory for saving the argument name");
    }
}
void bmic_op_arg_set_description(bmic_op_arg_t *arg, const char *description) {
    assert(arg != NULL);

    if (asprintf(&arg->description, "%s", description) == -1) {
        logger_t logger = gru_logger_get();
        logger(FATAL, "Unable to allocate memory for saving the argument description");
    }
}


void bmic_op_arg_set_type(bmic_op_arg_t *arg, const char *type) {
    assert(arg != NULL);

    if (asprintf(&arg->type, "%s", type) == -1) {
        logger_t logger = gru_logger_get();
        logger(FATAL, "Unable to allocate memory for saving the argument typename");
    }
}
