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

#include <stdbool.h>

#include <json-c/json.h>

#include <common/gru_alloc.h>
#include <common/gru_status.h>
#include <collection/gru_list.h>
#include <collection/gru_tree.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum bmic_json_data_type_t_ {
    INTEGER,
    STRING,
    BOOLEAN,
    DOUBLE,
    NULL_TYPE,
    ARRAY,
    OBJECT,
} bmic_json_data_type_t;

typedef union bmic_json_data_t_ {
    int64_t number;
    char *str;
    bool value;
    double d;
    gru_list_t *list;
    gru_tree_node_t *object;
} bmic_json_data_t;

typedef struct bmic_json_value_t_ {
    bmic_json_data_type_t type;
    bmic_json_data_t data;
} bmic_json_value_t;

typedef struct bmic_json_t_ {
    json_object *obj;
} bmic_json_t;

typedef int(*bmic_match_cond)(const char *keyname, const char *value);

bmic_json_t *bmic_json_init(const char *data, gru_status_t *status);
void bmic_json_destroy(bmic_json_t **json);

void bmic_json_find_first(const bmic_json_t *json, const char *keyname,
                          bmic_json_value_t *ret);
void bmic_json_find_cond(const bmic_json_t *json, const char *keyname,
                         bmic_match_cond condition,
                         bmic_json_value_t *ret);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_FORMAT_JSON_H */
