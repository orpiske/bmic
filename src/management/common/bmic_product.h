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

#include "base/common/bmic_object.h"
#include "base/common/bmic_cap_info.h"

#include "bmic_handle.h"
#include "bmic_discovery_hint.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bmic_product_info_t_ {
    char name[64];
    char version[16];
} bmic_product_info_t;

typedef enum bmic_exchange_type_t_ {
    EX_CAP_LIST,
    EX_CAP_ENTRY,
} bmic_exchange_type_t;

typedef union bmic_exchange_payload_t_ {
    const bmic_cap_info_t *capinfo;
} bmic_exchange_payload_t;

typedef struct bmic_exchange_t_ {
    bmic_exchange_type_t type;
    const bmic_object_t *root;
    const bmic_object_t *data_ptr;
    bmic_exchange_payload_t payload;
} bmic_exchange_t;

typedef const char *(*bmic_management_url_fn)(const bmic_discovery_hint_t *hint);

typedef bmic_handle_t *(*bmic_management_api_init_fn)(const char *base_url,
                                 bmic_credentials_t *credentials,
                                 gru_status_t *status);
typedef void(*bmic_management_api_cleanup_fn)(bmic_handle_t **handle);

typedef bmic_product_info_t *(*bmic_management_api_info_fn)(bmic_handle_t *handle,
        gru_status_t *status);
typedef const bmic_exchange_t *(*bmic_management_api_cap_fn)(bmic_handle_t *handle, 
        gru_status_t *status);
typedef const bmic_exchange_t *(*bmic_management_api_attr_read_fn)(bmic_handle_t *handle,
        const bmic_exchange_t *capabilities, const char *name,
        gru_status_t *status);
typedef const bmic_exchange_t *(*bmic_management_api_queue_attribute_read_fn)(bmic_handle_t *handle,
        const bmic_exchange_t *capabilities, const char *name,
        gru_status_t *status, const char *queue);

typedef const gru_list_t *(*bmic_management_api_attr_list_fn)(bmic_handle_t *handle,
        const bmic_exchange_t *cap, gru_status_t *status);


typedef struct bmic_api_interface_t_ {
    char name[64];
    char version[16];

    bmic_management_url_fn base_url;
    bmic_management_api_init_fn api_init;
    bmic_management_api_cleanup_fn api_cleanup;
    
    bmic_management_api_info_fn product_info;
    bmic_management_api_cap_fn load_capabilities;
    bmic_management_api_attr_read_fn attribute_read;
    bmic_management_api_attr_list_fn attribute_list;
    bmic_management_api_queue_attribute_read_fn queue_attribute_read;

} bmic_api_interface_t;



bmic_api_interface_t *bmic_api_interface_new(const char *name, const char *version,
        gru_status_t *status);
void bmic_api_interface_destroy(bmic_api_interface_t **product);

void bmic_product_registry_init(gru_status_t *status);
void bmic_product_registry_destroy();
void bmic_product_registry_add(const bmic_api_interface_t *product, gru_status_t *status);

const gru_list_t *bmic_product_registry();

static inline void bmic_exchange_destroy(bmic_exchange_t **ptr) {
    bmic_object_destroy((bmic_object_t **) &(*ptr)->root);
    gru_dealloc((void **) ptr);
}


#ifdef __cplusplus
}
#endif

#endif /* BMIC_PRODUCT_H */
