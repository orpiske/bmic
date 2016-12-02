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
#ifndef BMIC_ARTEMIS_JSON_H
#define BMIC_ARTEMIS_JSON_H

#include <common/gru_status.h>
#include <common/gru_alloc.h>

#include "base/format/bmic_json.h"

/**
 * @brief this is an interim solution for writing json data. I have to think
 * more about it, but I have to get the code working. I will think more about 
 * this in the future.
 */

#define CREATE_CORE_QUEUE_SIG "createQueue(java.lang.String,java.lang.String)"
#define DESTROY_CORE_QUEUE_SIG "destroyQueue(java.lang.String)"

#ifdef __cplusplus
extern "C" {
#endif

void bmic_artemis_json_create_queue(const bmic_object_t *op, bmic_json_t *json, 
                                    const char *name);


#ifdef __cplusplus
}
#endif

#endif /* BMIC_ARTEMIS_JSON_H */

