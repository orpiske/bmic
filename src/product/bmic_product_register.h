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
#ifndef BMIC_PRODUCT_REGISTER_H
#define BMIC_PRODUCT_REGISTER_H

#include "management/common/bmic_product.h"
#include "product/activemq/bmic_activemq.h"
#include "product/artemis/bmic_artemis.h"

#ifdef __cplusplus
extern "C" {
#endif

void bmic_product_register(gru_status_t *status);
void bmic_product_unregister();

#ifdef __cplusplus
}
#endif

#endif /* BMIC_PRODUCT_REGISTER_H */
