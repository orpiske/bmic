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

#include "base/common/bmic_data.h"
#include "base/format/bmic_json.h"

#include "management/common/bmic_handle.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file Internal API stuff
 */

/**
 * Lower level IO from the management interface
 * @param handle the BMIC handle for the interface
 * @param path the path within the interface
 * @param reply the storage for the reply
 * @param status the status for the I/O call
 */
void bmic_api_io_read(bmic_handle_t *handle, const char *path,
                                    bmic_data_t *reply, gru_status_t *status);

/**
 * Parses a JSON text data and transform into a bmic_object_t
 * @param str The JSON text data
 * @param status the status for the parsing
 * @return A pointer to a new object of type bmic_object_t
 */
bmic_object_t *bmic_api_parse_json(const char *str, gru_status_t *status);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_API_IO_H */

