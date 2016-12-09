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

#ifndef BMIC_JAMQ6_H
#define BMIC_JAMQ6_H

#include "bmic_activemq.h"
#include "bmic_jamq6_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

bmic_api_interface_t *bmic_jamq6_product(gru_status_t *status);
const char *bmic_jamq6_base_url(const bmic_discovery_hint_t *hint);
bmic_product_info_t *bmic_jamq6_product_info(bmic_handle_t *handle,
                                                gru_status_t *status);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_JAMQ6_H */

