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
#ifndef BMIC_JOLOKIA_IO_H
#define BMIC_JOLOKIA_IO_H

#include <stdlib.h>
#include <stdbool.h>

#include "base/common/bmic_object.h"
#include "base/format/bmic_json.h"
#include "base/transport/bmic_endpoint.h"

#include "management/common/bmic_handle.h"

#ifdef __cplusplus
extern "C" {
#endif

bool bmic_jolokia_io_exec(bmic_handle_t *handle,
                                            bmic_json_t *json,
                                            gru_status_t *status);


#ifdef __cplusplus
}
#endif

#endif /* BMIC_JOLOKIA_IO_H */

