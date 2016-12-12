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

#ifndef BMIC_MANAGEMENT_API_H
#define BMIC_MANAGEMENT_API_H

#include "base/common/bmic_object.h"
#include "base/common/bmic_list.h"


#include "bmic_handle.h"
#include "bmic_exchange.h"
#include "bmic_discovery_hint.h"
#include "bmic_product_info.h"
#include "bmic_queue_stat.h"

#include "bmic_complements_java.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef const char *(*bmic_management_url_fn)(const bmic_discovery_hint_t *hint);

typedef bmic_handle_t *(*bmic_management_api_init_fn)(const char *base_url,
        bmic_credentials_t *credentials,
        gru_status_t *status);
typedef void(*bmic_management_api_cleanup_fn)(bmic_handle_t **handle);

typedef bmic_product_info_t *(*bmic_management_api_info_fn)(bmic_handle_t *handle,
        const bmic_exchange_t *capabilities,
        gru_status_t *status);
typedef const bmic_exchange_t *(*bmic_management_api_cap_fn)(bmic_handle_t *handle,
        gru_status_t *status);
typedef const bmic_exchange_t *(*bmic_management_api_attr_read_fn)(bmic_handle_t *handle,
        const bmic_exchange_t *capabilities, const char *name,
        gru_status_t *status);
typedef const bmic_exchange_t *(*bmic_management_api_queue_attribute_read_fn)(bmic_handle_t *handle,
        const bmic_exchange_t *capabilities, const char *name,
        gru_status_t *status, const char *queue);

typedef const bmic_list_t *(*bmic_management_api_attr_list_fn)(bmic_handle_t *handle,
        const bmic_exchange_t *cap, gru_status_t *status);
typedef const bmic_list_t *(*bmic_management_api_op_list_fn)(bmic_handle_t *handle,
        const bmic_exchange_t *cap, gru_status_t *status);

// Queue functions
typedef bool (*bmic_management_api_queue_create_fn)(bmic_handle_t *handle,
        const bmic_exchange_t *cap,
        const char *name,
        gru_status_t *status);

typedef bool (*bmic_management_api_queue_delete_fn)(bmic_handle_t *handle,
        const bmic_exchange_t *cap,
        const char *name,
        gru_status_t *status);

typedef const bmic_list_t *(*bmic_management_api_queue_list_fn)(bmic_handle_t *handle, 
        const bmic_exchange_t *cap, gru_status_t *status);

typedef bmic_queue_stat_t (*bmic_management_api_queue_stats_fn)(bmic_handle_t *handle,
        const bmic_exchange_t *cap,
        const char *name,
        gru_status_t *status);

typedef struct bmic_api_interface_t_ {
    char name[64];
    char version[16];

    bmic_management_url_fn base_url;
    bmic_management_api_init_fn api_init;
    bmic_management_api_cleanup_fn api_cleanup;

    bmic_management_api_info_fn product_info;
    bmic_management_api_cap_fn capabilities_load;
    bmic_management_api_attr_read_fn attribute_read;
    bmic_management_api_attr_list_fn attribute_list;
    bmic_management_api_queue_attribute_read_fn queue_attribute_read;
    bmic_management_api_op_list_fn operation_list;
    bmic_management_api_queue_create_fn queue_create;
    bmic_management_api_queue_delete_fn queue_delete;
    bmic_management_api_queue_list_fn queue_list;
    bmic_management_api_queue_stats_fn queue_stats;
    
    bmic_complements_java_api_t java; 
} bmic_api_interface_t;


#ifdef __cplusplus
}
#endif

#endif /* BMIC_MANAGEMENT_API_H */

