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

bmic_json_t *bmic_json_init(const char *data, gru_status_t *status)
{
    enum json_tokener_error jerr = json_tokener_success;

    bmic_json_t *ret = gru_alloc(sizeof (bmic_json_t), status);
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

void bmic_json_destroy(bmic_json_t **json)
{
    bmic_json_t *jobj = *json;

    if (jobj && jobj->obj) {
        json_object_put(jobj->obj);
    }

    gru_dealloc((void **) json);
}

/**
 * A DFS-like search over the json object
 * @param jobj
 * @param keyname
 * @param ret
 */
static void bmic_json_find_first_cond_int(const json_object *jobj, const char *keyname,
                                          bmic_match_cond condition, bmic_object_t *ret)
{
    enum json_type type;

    json_object_object_foreach((json_object *) jobj, key, val)
    {
        type = json_object_get_type(val);

        switch (type) {
        case json_type_object:
            bmic_json_find_first_cond_int(val, keyname, condition, ret);
            ret->type = OBJECT; 
            break;
        case json_type_array:
            ret->type = ARRAY;
            // TODO: implement arrays
            break;
        case json_type_null:
            if (condition(key, keyname) == 0) {
                ret->type = NULL_TYPE;
                ret->data.str = NULL;

                return;
            }

            break;
        case json_type_string:
            if (condition(key, keyname) == 0) {
                ret->type = STRING;
                asprintf(&ret->data.str, "%s", json_object_get_string(val));
                return;
            }

            break;
        case json_type_int:
            if (condition(key, keyname) == 0) {
                ret->type = INTEGER;
                ret->data.number = json_object_get_int(val);

                return;
            }

            break;
        case json_type_boolean:
            if (condition(key, keyname) == 0) {
                ret->type = BOOLEAN;
                ret->data.value = json_object_get_boolean(val);

                return;
            }

            break;
        case json_type_double:
            if (condition(key, keyname) == 0) {
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

/**
 * A DFS-like search over the json object
 * @param jobj
 * @param keyname
 * @param ret
 */
static void bmic_json_find_first_int(const json_object *jobj, const char *keyname,
                                     bmic_object_t *ret)
{
    bmic_json_find_first_cond_int(jobj, keyname, strncmp, ret);

}

void bmic_json_find_first(const bmic_json_t *json, const char *keyname,
                          bmic_object_t *ret)
{
    return bmic_json_find_first_int(json->obj, keyname, ret);
}

void bmic_json_find_cond(const bmic_json_t *json, const char *keyname,
                         bmic_match_cond condition,
                         bmic_object_t *ret)
{
    return bmic_json_find_first_cond_int(json->obj, keyname, condition, ret);
}