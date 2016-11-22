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

#ifndef BMIC_CAPLIST_H
#define BMIC_CAPLIST_H

#include <common/gru_status.h>
#include <common/gru_alloc.h>

#include <collection/gru_list.h>

#include "bmic_cap_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef gru_list_t bmic_caplist_t;

bmic_caplist_t *bmic_caplist_new(gru_status_t *status);
void bmic_caplist_destroy(bmic_caplist_t **ptr);
        
#ifdef __cplusplus
}
#endif

#endif /* BMIC_CAPLIST_H */

