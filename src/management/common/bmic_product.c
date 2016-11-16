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
#include "bmic_product.h"

static gru_list_t *registry = NULL;

bmic_api_interface_t *bmic_api_interface_new(const char *name, const char *version,
                                  gru_status_t *status) {
    bmic_api_interface_t *ret = gru_alloc(sizeof(bmic_api_interface_t), status);
    gru_alloc_check(ret, NULL);


    snprintf(ret->name, sizeof(ret->name) - 1, "%s", name);
    snprintf(ret->version, sizeof(ret->version) - 1, "%s", version);

    return ret;
}

void bmic_exchange_destroy(bmic_exchange_t **ptr) {
    bmic_exchange_t *ex = *ptr;
    
    if (ex == NULL) {
        return;
    }
    
    bmic_object_destroy((bmic_object_t **) &ex->root);
    if (ex->type == EX_CAP_ENTRY) {
        bmic_cap_info_destroy((bmic_cap_info_t **) &ex->payload.capinfo);
    }
    
    gru_dealloc((void **) ptr);
}


void bmic_api_interface_destroy(bmic_api_interface_t **product) {
    bmic_api_interface_t *prod = *product;

    if (!prod) {
        return;
    }

    gru_dealloc((void **)product);
}

void bmic_product_registry_init(gru_status_t *status) {
    if (registry == NULL) {
        registry = gru_list_new(status);

        if (registry == NULL) {
            return;
        }
    }
    else {
        gru_status_set(status, GRU_FAILURE,
                       "A product registry has already been initialized");

        return;
    }
}

void bmic_product_registry_destroy() {
    gru_list_destroy(&registry);
}


void bmic_product_registry_add(const bmic_api_interface_t *product, gru_status_t *status) {
    gru_list_append(registry, product);
}


const gru_list_t *bmic_product_registry() {
    return (const gru_list_t *) registry;
}
