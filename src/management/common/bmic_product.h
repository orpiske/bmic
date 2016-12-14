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
#ifndef BMIC_PRODUCT_H
#define BMIC_PRODUCT_H

#include <common/gru_status.h>

#include "base/common/bmic_object.h"
#include "base/common/bmic_list.h"

#include "bmic_exchange.h"
#include "bmic_management_api.h"

#ifdef __cplusplus
extern "C" {
#endif

bmic_api_interface_t *bmic_api_interface_new(const char *name, const char *version,
        gru_status_t *status);
void bmic_api_interface_destroy(bmic_api_interface_t **product);

void bmic_product_registry_init(gru_status_t *status);
void bmic_product_registry_destroy();
void bmic_product_registry_add(const bmic_api_interface_t *product, gru_status_t *status);

const gru_list_t *bmic_product_registry();

void bmic_exchange_destroy(bmic_exchange_t **ptr);
void bmic_exchange_destroy_const(const bmic_exchange_t **ptr);


#ifdef __cplusplus
}
#endif

#endif /* BMIC_PRODUCT_H */
