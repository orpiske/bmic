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

#ifndef BMIC_TRANSPORT_HTTP_H
#define BMIC_TRANSPORT_HTTP_H

#include <common/gru_status.h>

#include <curl/curl.h>

#include "bmic_transport.h"
#include "base/common/bmic_data.h"

#define HTTP_STATUS_OK 200

#ifdef __cplusplus
extern "C" {
#endif
    
void bmic_endpoint_http_begin(bmic_endpoint_t *ep, gru_status_t *status);
void bmic_endpoint_http_terminate(bmic_endpoint_t *ep, gru_status_t *status);

void bmic_endpoint_http_read(const bmic_endpoint_t *, bmic_data_t *payload, 
        bmic_data_t *data, bmic_endpoint_status_t *status); 

void bmic_endpoint_http_write(const bmic_endpoint_t *, bmic_data_t *payload, 
        bmic_data_t *data, bmic_endpoint_status_t *status); 


#ifdef __cplusplus
}
#endif

#endif /* BMIC_TRANSPORT_HTTP_H */

