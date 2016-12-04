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
#ifndef BMIC_JAVA_H
#define BMIC_JAVA_H

#include <common/gru_status.h>
#include <common/gru_alloc.h>

#include "management/common/bmic_handle.h"
#include "management/common/bmic_api_io.h"
#include "management/common/bmic_internal_payloads.h"
#include "management/discovery/bmic_discovery.h"
#include "management/interfaces/jolokia/bmic_jolokia.h"
#include "management/interfaces/jolokia/bmic_jolokia_io.h"

#include "bmic_java_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Read a single an attribute from the MI
 * @param handle
 * @param capabilities
 * @param name
 * @param status
 * @return 
 */
const bmic_exchange_t *bmic_java_attribute_read(bmic_handle_t *handle,
        const bmic_exchange_t *capabilities, const char *name,
        gru_status_t *status);


#ifdef __cplusplus
}
#endif

#endif /* BMIC_JAVA_H */

