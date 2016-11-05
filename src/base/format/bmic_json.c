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
                                          bmic_match_cond condition, bmic_object_t *parent_object)
{
    enum json_type type;

    json_object_object_foreach((json_object *) jobj, key, val)
    {
        // fprintf(stdout, "Running from %s\n", key);
        type = json_object_get_type(val);

        switch (type) {
        case json_type_object: {
            fprintf(stdout, "Entering the object %s\n", key);
            bmic_object_t *child = bmic_object_new(key, NULL);
            bmic_json_find_first_cond_int(val, keyname, condition, child);
            bmic_object_add_object(parent_object, child);
            
            break;
        }
        case json_type_array:
            fprintf(stdout, "Skipping the array %s\n", key);
            break;
        case json_type_null:
            if (condition(key, keyname) == 0) {
                bmic_object_set_null(parent_object);
                return;
            }

            break;
        case json_type_string:
            //fprintf(stdout, "Setting string for %s\n", key);
            if (condition(key, keyname) == 0) {
                bmic_object_set_string(parent_object, json_object_get_string(val));
                
               return;
            }

            break;
        case json_type_int:
            fprintf(stdout, "Int value for %s with value %d\n", key, 
                    json_object_get_int(val));
            
            bmic_object_t *child2 = bmic_object_new(key, NULL);
            
            bmic_object_add_object(parent_object, child2);
            
            if (condition(key, keyname) == 0) {
                bmic_object_set_integer(child2, json_object_get_int(val));
                return;
            }

            break;
        case json_type_boolean:
            fprintf(stdout, "Setting boolean for %s\n", key);
            if (condition(key, keyname) == 0) {
                bmic_object_set_boolean(parent_object, json_object_get_boolean(val));

                return;
            }

            break;
        case json_type_double:
            fprintf(stdout, "Setting double for %s\n", key);
            if (condition(key, keyname) == 0) {
                bmic_object_set_double(parent_object, json_object_get_double(val));

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


/**
 * A DFS-like search over the json object
 * @param jobj
 * @param keyname
 * @param ret
 */
static void bmic_json_transform_int(const json_object *jobj, bmic_object_t *parent)
{
    enum json_type type;

    json_object_object_foreach((json_object *) jobj, key, val)
    {
        type = json_object_get_type(val);

        bmic_object_t *child = bmic_object_new(key, NULL);
        switch (type) {
        case json_type_object: {            
            bmic_json_transform_int(val, child);
            
            break;
        }
        case json_type_array:

            
            for (int i = 0; i < json_object_array_length(val); i++) {
                json_object *tmp = json_object_array_get_idx(val, i); 
                                
                bmic_object_t *element = bmic_object_new(key, NULL);
                bmic_json_transform_int(tmp, element);
                bmic_object_add_list_element(child, element);
            }
            

            break;
        case json_type_null:
            bmic_object_set_null(parent);
            
            break;
        case json_type_string:
            bmic_object_set_string(child, json_object_get_string(val));
            
            break;
        case json_type_int:
            bmic_object_set_integer(child, json_object_get_int(val));
            

            break;
        case json_type_boolean:
            bmic_object_set_boolean(child, json_object_get_boolean(val));


            break;
        case json_type_double:
            bmic_object_set_double(child, json_object_get_double(val));

            break;
        default:
            break;
        }
        bmic_object_add_object(parent, child);
    }
}

void bmic_json_transform(const bmic_json_t *jobj, bmic_object_t *ret) {
    return bmic_json_transform_int(jobj->obj, ret);

}