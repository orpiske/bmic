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
#ifndef BMIC_ACTIVEMQ_H
#define BMIC_ACTIVEMQ_H

#include <common/gru_status.h>
#include <common/gru_alloc.h>

#include "base/common/bmic_credentials.h"
#include "base/transport/bmic_transport.h"
#include "base/transport/bmic_endpoint_http.h"

#include "management/common/bmic_handle.h"
#include "management/discovery/bmic_discovery.h"

#ifdef __cplusplus
extern "C" {
#endif

bmic_product_t *bmic_activemq_product(gru_status_t *status);
const char *bmic_activemq_base_url(bmic_discovery_hint_t *hint);
bmic_handle_t *bmic_activemq_init(const char *base_url, 
                                 bmic_credentials_t *credentials, 
                                 gru_status_t *status);


#ifdef __cplusplus
}
#endif

#endif /* BMIC_ACTIVEMQ_H */
