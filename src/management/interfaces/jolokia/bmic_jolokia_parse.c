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
#include "bmic_jolokia_parse.h"

bmic_object_t *bmic_api_parse_json(const char *str, gru_status_t *status) {
    bmic_object_t *root = bmic_object_new_root(status);
    if (!root) {
        return NULL;
    }
    
    bmic_json_t *json = bmic_json_init(str, status);
    if (json == NULL || status->code != GRU_SUCCESS) {
        bmic_object_destroy(&root);
        return NULL;
    }
    
    bmic_json_transform(json, root);
    bmic_json_destroy(&json);
    
    return root;
}