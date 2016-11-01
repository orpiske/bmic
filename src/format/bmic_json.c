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
#include "bmic_json.h"

bmic_json_t *bmic_json_init(const char *data, gru_status_t *status) {
    enum json_tokener_error jerr = json_tokener_success;
    
    bmic_json_t *ret = gru_alloc(sizeof(bmic_json_t), status);
    gru_alloc_check(ret, NULL);
    
    json_object *json = json_tokener_parse_verbose(data, &jerr);

    if (jerr != json_tokener_success) {
        gru_status_set(status, GRU_FAILURE, "Unable to parse the JSON data");
        
        bmic_json_destroy(&ret);
        return NULL;
    }
    
    ret->obj = json;
    return ret;
}

void bmic_json_destroy(bmic_json_t **json) {
    json_object *jobj = &json;
    
    if (jobj) {
        json_object_put(jobj);
    }
    
    gru_dealloc(&json);
}

/**
 * A DFS-like search over the json object 
 * @param jobj
 * @param keyname
 * @param ret
 */
static void bmic_json_find_first_int(const json_object *jobj, const char *keyname, 
                          bmic_json_value_t *ret)
{
    enum json_type type;

    json_object_object_foreach(jobj, key, val)
    {
        type = json_object_get_type(val);
        
        switch (type) {
        case json_type_object: 
            bmic_json_find_first_int(val, keyname, ret);
            break;
        case json_type_array: 
            // TODO: implement arrays
            break;
        case json_type_null: 
            if (strcmp(key, keyname) == 0) {
                ret->type = NULL_TYPE;
                ret->data.str = NULL;
                
                return;
            }
            
            break;
        case json_type_string: 
            if (strcmp(key, keyname) == 0) {
                ret->type = STRING;
                asprintf(&ret->data.str, "%s", json_object_get_string(val));
                return;
            }
            
            break;
        case json_type_int: 
            if (strcmp(key, keyname) == 0) {
                ret->type = INTEGER;
                ret->data.number = json_object_get_int(val);

                return;
            }
            
            break;
        case json_type_boolean: 
            if (strcmp(key, keyname) == 0) {
                ret->type = BOOLEAN;
                ret->data.value = json_object_get_boolean(val);

                return;
            }
            
            break;
        case json_type_double: 
            if (strcmp(key, keyname) == 0) {
                ret->type = DOUBLE;
                ret->data.d = json_object_get_double(val);

                return;
            }
            
            break;
        default: 
            printf("Invalid type for %s = %s\n", key, json_object_get_string(val));
            break;
        }
    }
}


void bmic_json_find_first(const bmic_json_t *json, const char *keyname, 
                          bmic_json_value_t *ret)
{
    return bmic_json_find_first_int(json->obj, keyname, ret);
}