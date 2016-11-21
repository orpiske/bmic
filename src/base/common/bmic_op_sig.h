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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bmic_op_sig_t_ {
    gru_list_t *args;
} bmic_op_sig_t;

bmic_op_sig_t *bmic_op_sig_new(gru_status_t *status);
void bmic_op_sig_destroy(bmic_op_sig_t **ptr);

void bmic_op_sig_add_arg(bmic_op_sig_t *sigs, const bmic_op_arg_t *arg);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_OP_SIG_H */

