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
#ifndef BMIC_ENDPOINT_H
#define BMIC_ENDPOINT_H

#include <common/gru_alloc.h>

#include "base/common/bmic_credentials.h"

#ifdef __cplusplus
extern "C" {
#endif
    
typedef struct bmic_endpoint_t_ {
    const char *url;
    char *path;
    bmic_credentials_t *credentials;
    void *handle;
} bmic_endpoint_t;

typedef struct bmic_endpoint_status_t_ {
    gru_status_t *status; 
    int32_t epcode; 
    char *message;
} bmic_endpoint_status_t;

bmic_endpoint_t *bmic_endpoint_init(const char *url, const char *username, 
        const char *password, gru_status_t *status);

void bmic_endpoint_destroy(bmic_endpoint_t **ep);

void bmic_endpoint_set_credentials(bmic_endpoint_t *ep, 
    bmic_credentials_t *credentials, gru_status_t *status);

void bmic_endpoint_set_path(bmic_endpoint_t *ep, const char *path);
void bmic_endpoint_reset_path(bmic_endpoint_t *ep);

// void bmic_endpoint_status()

#ifdef __cplusplus
}
#endif

#endif /* BMIC_ENDPOINT_H */

