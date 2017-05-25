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

bmic_json_t bmic_json_new(gru_status_t *status) {
	bmic_json_t ret = {0};

	ret.obj = json_object_new_object();
	if (!ret.obj) {
		gru_status_set(status, GRU_FAILURE, "Unable to create the json object");
	}

	return ret;
}

bmic_json_t bmic_json_init(const char *data, gru_status_t *status) {
	enum json_tokener_error jerr = json_tokener_success;

	bmic_json_t ret = {0};

	ret.obj = json_tokener_parse_verbose(data, &jerr);

	if (jerr != json_tokener_success) {
		gru_status_set(status, GRU_FAILURE, "Unable to parse the JSON data");

		bmic_json_cleanup(ret);
	}

	return ret;
}

void bmic_json_cleanup(bmic_json_t json) {
	if (json.obj) {
		json_object_put(json.obj);
	}
}

static void bmic_json_transform_simple_obj(
	bmic_object_t *obj, enum json_type type, json_object *val) {
	switch (type) {
		case json_type_string:
			bmic_object_set_string(obj, json_object_get_string(val));

			break;
		case json_type_int:
			bmic_object_set_integer(obj, json_object_get_int64(val));

			break;
		case json_type_boolean:
			bmic_object_set_boolean(obj, json_object_get_boolean(val));

			break;
		case json_type_double:
			bmic_object_set_double(obj, json_object_get_double(val));

			break;
		default:
			break;
	}
}

/**
 * A DFS-like search over the json object
 * @param jobj
 * @param keyname
 * @param ret
 */
static void bmic_json_transform_int(const json_object *jobj, bmic_object_t *parent) {
	enum json_type type;

	json_object_object_foreach((json_object *) jobj, key, val) {
		type = json_object_get_type(val);

		bmic_object_t *child = bmic_object_new(key, NULL);
		bmic_object_add_object(parent, child);

		switch (type) {
			case json_type_object: {
				bmic_json_transform_int(val, child);

				break;
			}
			case json_type_array: {
				for (int i = 0; i < json_object_array_length(val); i++) {
					json_object *tmp = json_object_array_get_idx(val, i);
					bmic_object_t *element = bmic_object_new(key, NULL);

					bmic_object_add_list_element(child, element);

					enum json_type arr_type = json_object_get_type(tmp);
					if (arr_type == json_type_object) {
						bmic_json_transform_int(tmp, element);
					} else {
						bmic_json_transform_simple_obj(element, arr_type, tmp);
					}
				}

				break;
			}
			case json_type_null:
				bmic_object_set_null(parent);

				break;
			default:
				bmic_json_transform_simple_obj(child, type, val);
				break;
		}
	}
}

void bmic_json_transform(const bmic_json_t *jobj, bmic_object_t *ret) {
	assert(jobj != NULL && ret != NULL);

	bmic_json_transform_int(jobj->obj, ret);
}

bmic_data_t bmic_json_to_data(const bmic_json_t *jobj, gru_status_t *status) {
	bmic_data_t data = {0};
	const size_t MAX_SIZE = 1024 * 1024 * 5;

	data.data = (void *) json_object_to_json_string(jobj->obj);

	if (data.data != NULL) {
		data.size = strnlen((char *) data.data, MAX_SIZE);
	} else {
		data.size = 0;
	}

	return data;
}