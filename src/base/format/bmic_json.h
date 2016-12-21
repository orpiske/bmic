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
#ifndef BMIC_FORMAT_JSON_H
#define BMIC_FORMAT_JSON_H

#include <assert.h>
#include <stdbool.h>

#if defined(__DEBIAN_DISTRO__) || defined(__UBUNTU_DISTRO__)
#include <json/json.h>
#else
#include <json-c/json.h>
#endif

#include <common/gru_alloc.h>
#include <common/gru_status.h>

#include "base/common/bmic_data.h"
#include "base/common/bmic_object.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bmic_json_t_ { json_object *obj; } bmic_json_t;

typedef int (*bmic_match_cond)(const char *keyname, const char *value);

bmic_json_t *bmic_json_new(gru_status_t *status);
bmic_json_t *bmic_json_init(const char *data, gru_status_t *status);
void bmic_json_destroy(bmic_json_t **json);

void bmic_json_transform(const bmic_json_t *jobj, bmic_object_t *ret);
bmic_data_t bmic_json_to_data(const bmic_json_t *jobj, gru_status_t *status);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_FORMAT_JSON_H */
