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
#ifndef BMIC_OP_SIG_H
#define BMIC_OP_SIG_H

#include <common/gru_alloc.h>
#include <common/gru_status.h>

#include <collection/gru_list.h>

#include "bmic_op_arg.h"

/**
 * @brief Operation signatures
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bmic_op_sig_t_ {
	gru_list_t *args;
	char *ret;
	char *description;
} bmic_op_sig_t;

/**
 * Create a new operation signature
 * @param status
 * @return
 */
bmic_op_sig_t *bmic_op_sig_new(gru_status_t *status);

/**
 * Destroy a signature object releasing all allocated memory
 * @param ptr the pointer to destroy
 */
void bmic_op_sig_destroy(bmic_op_sig_t **ptr);

/**
 * Adds an argument to the method signature
 * @param sig the signature
 * @param arg the argument
 */
void bmic_op_sig_add_arg(bmic_op_sig_t *sig, const bmic_op_arg_t *arg);

/**
 * Sets the description
 * @param sig the signature object
 * @param description the description to set
 */
void bmic_op_sig_set_description(bmic_op_sig_t *sig, const char *description);

/**
 * Sets the return type
 * @param sig the signature object
 * @param ret the return type text
 */
void bmic_op_sig_set_ret(bmic_op_sig_t *sig, const char *ret);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_OP_SIG_H */
