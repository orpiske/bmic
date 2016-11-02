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
#include <collection/gru_list.h>

#include "bmic_handle.h"
#include "bmic_discovery_hint.h"

#ifdef __cplusplus
extern "C" {
#endif



typedef const char *(*bmic_product_base_url_fn)(bmic_discovery_hint_t *hint);

typedef bmic_handle_t *(*bmic_product_init_fn)(const char *base_url, 
                                 bmic_credentials_t *credentials, 
                                 gru_status_t *status);

typedef struct bmic_product_t_ {
    char *name;
    char *version;
    
    bmic_product_base_url_fn base_url;
    bmic_product_init_fn product_init;
} bmic_product_t;

bmic_product_t *bmic_product_init(const char *name, const char *version, 
        gru_status_t *status);
void bmic_product_destroy(bmic_product_t **product);

void bmic_product_registry_init(gru_status_t *status);
void bmic_product_registry_destroy();
void bmic_product_registry_add(const bmic_product_t *product, gru_status_t *status);

const gru_list_t *bmic_product_registry();


#ifdef __cplusplus
}
#endif

#endif /* BMIC_PRODUCT_H */

