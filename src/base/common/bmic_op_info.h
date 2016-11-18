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
#ifndef BMIC_OP_INFO_H
#define BMIC_OP_INFO_H

#include <common/gru_alloc.h>
#include <common/gru_status.h>

#include <collection/gru_list.h>

#include "bmic_op_arg.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bmic_op_info_t_ {
    char *name;
    gru_list_t *args;
    char *ret;
    char *description;
} bmic_op_info_t;


/**
 * Creates a new operation info object
 * @param status status object to contain error information
 * @return NULL if unable to allocate memory or a pointer to a new 
 * bmic_op_arg_t structure
 */
bmic_op_info_t *bmic_op_info_new(gru_status_t *status);

/**
 * Destroys an operation info object
 * @param ptr the object to destroy
 */
void bmic_op_info_destroy(bmic_op_info_t **ptr);


/**
 * Sets the name
 * @param op the operation object to set the name
 * @param name the name to set
 */
void bmic_op_info_set_name(bmic_op_info_t *op, const char *name);

/**
 * Sets the description
 * @param op the operation object to set the name
 * @param description the description to set
 */
void bmic_op_info_set_description(bmic_op_info_t *op, const char *description);

/**
 * Sets the return type
 * @param op the operation object to set the name
 * @param type the name to set
 */
void bmic_op_info_set_ret(bmic_op_info_t *op, const char *typename);

/**
 * Adds and argument to the operation
 * @param op the operation object to set the name
 * @param arg the argument to add
 */
void bmic_op_info_add_arg(bmic_op_info_t *op, const bmic_op_arg_t *arg);



#ifdef __cplusplus
}
#endif

#endif /* BMIC_OP_INFO_H */

