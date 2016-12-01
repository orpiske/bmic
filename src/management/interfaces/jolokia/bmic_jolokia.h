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
#ifndef BMIC_JOLOKIA_H
#define BMIC_JOLOKIA_H

#include <common/gru_status.h>

#include "base/common/bmic_object.h"
#include "base/common/bmic_cap_info.h"
#include "base/common/bmic_op_arg.h"
#include "base/common/bmic_op_sig.h"
#include "base/common/bmic_op_info.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Given a node of operations, read them into the info object
 * @param obj
 * @param info
 * @note The expected formated for the transformed BMIC Object is: 
 *  "op": { "opname": { "args": [ { "name": "p1", "type": "java.lang.String", "desc": "" },
 */
void bmic_jolokia_translate_op(const bmic_object_t *obj,
                                         bmic_op_info_t *info,
                                        gru_status_t *status);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_JOLOKIA_H */

