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
#ifndef BMIC_TRANSPORT_H
#define BMIC_TRANSPORT_H

#include <stdio.h>
#include <stdlib.h>

#include <common/gru_status.h>

#include "bmic_data.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef void(*bmic_transport_read)(bmic_endpoint_t *, bmi_data_t *payload,
        bmi_data_t *data, gru_status_t *status);

typedef void(*bmic_transport_write)(bmic_endpoint_t *, bmi_data_t *payload,
        bmi_data_t *data, gru_status_t *status);

typedef struct bmic_vtransport_t_ {
    bmic_transport_read read;
    bmic_transport_write write;
} bmic_vtransport_t;


#ifdef __cplusplus
}
#endif

#endif /* BMIC_TRANSPORT_H */

