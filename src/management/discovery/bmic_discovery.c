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

static bool bmic_try_init(bmic_product_t *product, bmic_discovery_pair_t *pair) {
    const char *base_url = product->base_url(pair->hint);
       
    gru_status_t status = {0};
    
    // printf("Name: %s\n", product->name);
    // printf("Base URL: %s\n", base_url);
    bmic_handle_t *handle = product->product_init(base_url, pair->credentials, &status);
    bmic_product_info_t *info = product->product_info(handle, &status);
    
    if (info) {
        // printf("Obtained product version: %s\n", info->version);
        return true;
    }
    else {
        if (status.code != GRU_SUCCESS) { 
            printf("Error: %s\n", status.message);
        }
    }
    
    return false;
}


static bmic_product_info_t *bmic_discovery_registry_initializer(
            const gru_list_t *list, 
            bmic_discovery_pair_t *pair)
{
	gru_node_t *node = NULL;

	if (list == NULL) {
		return;
	}

	node = list->root;

	while (node) {
            bmic_product_t *product = gru_node_get_data_ptr(bmic_product_t *, node);
                        
            bool initialized = bmic_try_init(product, pair);
		
            if (initialized) {
                return product;
            }

            node = node->next;
	}
        
        return NULL;
}

bmic_product_t *bmic_discovery_run(const bmic_discovery_hint_t *hint,
                                   bmic_credentials_t *credentials,
        gru_status_t *status) 
{
    const gru_list_t *list = bmic_product_registry();
    bmic_discovery_pair_t pair = {0};
    
    pair.credentials = credentials;
    pair.hint = hint;
    
    return bmic_discovery_registry_initializer(list, &pair);
}