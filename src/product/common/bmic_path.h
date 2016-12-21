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
#ifndef BMIC_PATH_H
#define BMIC_PATH_H

#include <stdlib.h>

#include <common/gru_status.h>

#include "base/common/bmic_object.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Formats a path
 * @param op Operation (ie.: read, write, list, etc)
 * @param interface The management interface to set in the path
 * @param pkg Root package
 * @param capname Capability name
 * @param status status
 * @return The path as a malloc'd string whith must be free'd after use or NULL
 * in case of error
 */
const char *bmic_path_formatter(const char *op, const char *interface, const char *pkg,
	const char *capname, gru_status_t *status);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_PATH_H */
