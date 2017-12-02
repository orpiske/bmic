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

#ifndef BMIC_ARTEMIS24_H
#define BMIC_ARTEMIS24_H

#include <string/gru_alt.h>

#include "bmic_artemis.h"
#include "bmic_artemis24_defs.h"

#define ARTEMIS24_PRODUCT_NAME "artemis 2.4"
#define ARTEMIS24_PRODUCT_NAME_PRETTY "Artemis"
#define ARTEMIS24_API_VERSION "1.1.x"


#ifdef __cplusplus
extern "C" {
#endif

bmic_api_interface_t *bmic_artemis24_product(gru_status_t *status);
const char *bmic_artemis24_base_url(const bmic_discovery_hint_t *hint);

void bmic_artemis24_cleanup(bmic_handle_t **handle);

bmic_product_info_t *bmic_artemis24_product_info(
	bmic_handle_t *handle, const bmic_exchange_t *cap, gru_status_t *status);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_ARTEMIS24_H */
