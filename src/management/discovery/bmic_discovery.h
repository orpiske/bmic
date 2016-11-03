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
#ifndef BMIC_DISCOVERY_H
#define BMIC_DISCOVERY_H

#include <common/gru_alloc.h>
#include <common/gru_status.h>

#include "base/common/bmic_credentials.h"

#include "management/common/bmic_product.h"
#include "management/common/bmic_discovery_hint.h"

#ifdef __cplusplus
extern "C" {
#endif



/**
 * Runs product discovery on the given hint
 * @param hint A hint to find out details about the broker
 * @param credentials Optional credentials used to query the broker
 * @param status A status struct that will contain error data if the function failed
 * @return The product information
 */
bmic_product_t *bmic_discovery_run(const bmic_discovery_hint_t *hint, 
        bmic_credentials_t *credentials,
        gru_status_t *status);


#ifdef __cplusplus
}
#endif

#endif /* BMIC_DISCOVERY_H */

