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
#ifndef BMIC_OP_ARG_H
#define BMIC_OP_ARG_H

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include <common/gru_alloc.h>
#include <common/gru_status.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bmic_op_arg_t_ {
    char *name;
    char *type;
    char *description;
} bmic_op_arg_t;


/**
 * Creates a new argument object
 * @param status status object to contain error information
 * @return NULL if unable to allocate memory or a pointer to a new 
 * bmic_op_arg_t structure
 */
bmic_op_arg_t *bmic_op_arg_new(gru_status_t *status);


/**
 * Destroys the argument object and free's all memory used
 * @param ptr the object to free
 */
void bmic_op_arg_destroy(bmic_op_arg_t **ptr);

/**
 * Sets the name
 * @param arg the argument object to set the name
 * @param name the name to set
 */
void bmic_op_arg_set_name(bmic_op_arg_t *arg, const char *name);

/**
 * Sets the description
 * @param arg the argument object to set the name
 * @param description the description to set
 */
void bmic_op_arg_set_description(bmic_op_arg_t *arg, const char *description);

/**
 * Sets the argument type
 * @param arg the argument object to set the name
 * @param type the name to set
 */
void bmic_op_arg_set_type(bmic_op_arg_t *arg, const char *type);


#ifdef __cplusplus
}
#endif

#endif /* BMIC_OP_ARG_H */

