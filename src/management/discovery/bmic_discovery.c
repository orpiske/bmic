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

typedef struct bmic_discovery_pair_t_
{
    const bmic_discovery_hint_t *hint;
    bmic_credentials_t *credentials;
} bmic_discovery_pair_t;


static bool bmic_try_init(bmic_api_interface_t *api,
                          bmic_handle_t **outhandle,
                          bmic_discovery_pair_t *pair)
{
    logger_t logger = gru_logger_get();
    
    const char *base_url = api->base_url(pair->hint);

    gru_status_t status = {0};

    logger(DEBUG, "Initializing base URL from API");
    bmic_handle_t *handle = api->api_init(base_url, pair->credentials, &status);
    if (!handle) {
        gru_dealloc_string((char **) &base_url);
        return false;
    }

    logger(DEBUG, "Trying to load product capabilities");
    bmic_exchange_t *cap = api->capabilities_load(handle, &status); 
    if (!cap) {
        logger(DEBUG, "Unable to load product capabilities for %s: %s", 
               api->name, status.message);
        
        return false;
    }
    
    bmic_product_info_t *info = api->product_info(handle, cap, &status);

    if (info) {
        *outhandle = handle;
        gru_dealloc((void **) &info);
        gru_dealloc_string((char **) &base_url);
        bmic_exchange_destroy(&cap);
        return true;
    }

    bmic_exchange_destroy(&cap);
    gru_status_reset(&status);
    gru_dealloc((void **)&info);
    api->api_cleanup(&handle);
    gru_dealloc_string((char **) &base_url);
    return false;
}

static bmic_api_interface_t *bmic_discovery_registry_initializer(const gru_list_t *list,
                                                                bmic_handle_t **outhandle,
                                                                bmic_discovery_pair_t *pair)
{
    gru_node_t *node = NULL;

    if (list == NULL) {
        return NULL;
    }

    node = list->root;

    while (node) {
        bmic_api_interface_t *api = gru_node_get_data_ptr(bmic_api_interface_t, node);

        bool initialized = bmic_try_init(api, outhandle, pair);

        if (initialized) {
            return api;
        }

        node = node->next;
    }

    return NULL;
}

bmic_api_interface_t *bmic_discovery_run(const bmic_discovery_hint_t *hint,
                                   bmic_credentials_t *credentials,
                                   bmic_handle_t **outhandle,
                                   gru_status_t *status)
{
    const gru_list_t *list = bmic_product_registry();
    bmic_discovery_pair_t pair = {0};

    pair.credentials = credentials;
    pair.hint = hint;

    return bmic_discovery_registry_initializer(list, outhandle, &pair);
}
