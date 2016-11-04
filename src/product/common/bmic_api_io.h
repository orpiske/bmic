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
#ifndef BMIC_API_IO_H
#define BMIC_API_IO_H

#include <stdlib.h>

#include <common/gru_status.h>

#include "base/transport/bmic_data.h"
#include "base/format/bmic_json.h"

#include "management/common/bmic_handle.h"

#ifdef __cplusplus
extern "C" {
#endif


void bmic_api_io_read(bmic_handle_t *handle, const char *path,
                                    bmic_data_t *reply, gru_status_t *status);

void bmic_api_io_find_value(const bmic_data_t *reply, 
                                     bmic_json_value_t *value, 
                                     const char *name,
                                     gru_status_t *status);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_API_IO_H */

