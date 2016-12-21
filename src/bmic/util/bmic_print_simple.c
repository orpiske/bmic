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
#include "bmic_print_simple.h"

static void print_returned_from_list(const void *ptr, void *payload) {
	bmic_object_t *obj = (bmic_object_t *) ptr;

	switch (obj->type) {
		case STRING: {
			printf("%-35s %s\n", " ", obj->data.str);
			break;
		}
		case INTEGER: {
			printf("%-35s %" PRId64 "\n", " ", obj->data.number);
			break;
		}
		case BOOLEAN: {
			printf("%-35s %-25s\n", " ", (obj->data.value ? "true" : "false"));
			break;
		}
		case DOUBLE: {
			printf("%-35s %.4f\n", " ", obj->data.d);
			break;
		}
		case OBJECT: {
			bmic_object_for_each_child(obj, print_returned_from_list, (void *) payload);
			break;
		}
		default: { break; }
	}
}

void print_returned_object(const char *capname, const bmic_object_t *obj) {
	switch (obj->type) {
		case STRING: {
			printf("%35s %-25s\n", capname, obj->data.str);
			break;
		}
		case INTEGER: {
			printf("%35s %" PRId64 "\n", capname, obj->data.number);
			break;
		}
		case BOOLEAN: {
			printf("%35s %s\n", capname, (obj->data.value ? "true" : "false"));
			break;
		}
		case DOUBLE: {
			printf("%35s %.4f\n", capname, obj->data.d);
			break;
		}
		case NULL_TYPE: {
			printf("%35s (null)\n", capname);
			break;
		}
		case LIST: {
			printf("%35s \n", capname);

			bmic_object_for_each_child(obj, print_returned_from_list, (void *) capname);
			break;
		}
		case OBJECT:
		default: {
			printf("%35s \n", capname);
			bmic_object_for_each(obj, print_returned_from_list, (void *) capname);
			break;
		}
	}
}

void print_product_info(bmic_api_interface_t *api, bmic_product_info_t *info) {
	printf("The product name is %s\n", api->name);
	printf("The API version is %s\n", api->version);

	if (info) {
		printf("The product version is: %s\n", info->version);
	}
}