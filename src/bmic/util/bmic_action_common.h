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
#ifndef BMIC_ACTION_COMMON_H
#define BMIC_ACTION_COMMON_H

#include <stdbool.h>

#include <common/gru_status.h>

#include "management/common//bmic_management_api.h"
#include "management/common//bmic_handle.h"

#include "bmic_print_simple.h"

#ifdef __cplusplus
extern "C" {
#endif


void show_info(bmic_api_interface_t *api, bmic_handle_t *handle, const bmic_exchange_t *cap, 
        bool show, 
        gru_status_t *status);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_ACTION_COMMON_H */

