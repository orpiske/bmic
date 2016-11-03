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
#include "bmic_discovery.h"

typedef struct bmic_discovery_pair_t_ {
    bmic_discovery_hint_t *hint;
    bmic_credentials_t *credentials;
} bmic_discovery_pair_t;

void bmic_try_init(const gru_node_t *node, void *data) {
    bmic_product_t *product = gru_node_get_data_ptr(bmic_product_t *, node);
    bmic_discovery_pair_t *pair = (bmic_discovery_pair_t *) data;
    
    const char *base_url = product->base_url(pair->hint);
       
    gru_status_t status = {0};
    
    printf("Name: %s\n", product->name);
    printf("Base URL: %s\n", base_url);
    bmic_handle_t *handle = product->product_init(base_url, pair->credentials, &status);
    bmic_product_info_t *info = product->product_info(handle, &status);
    
    if (info) {
        printf("Obtained product version: %s\n", info->version);
    }
}

bmic_product_t *bmic_discovery_run(const bmic_discovery_hint_t *hint,
                                   bmic_credentials_t *credentials,
        gru_status_t *status) 
{
    const gru_list_t *list = bmic_product_registry();
    bmic_discovery_pair_t pair = {0};
    
    pair.credentials = credentials;
    pair.hint = hint;
    
    gru_list_for_each(list, bmic_try_init, &pair);
    
    return NULL;
}