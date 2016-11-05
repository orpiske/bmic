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
#include "bmic_object.h"

bmic_object_t *bmic_object_new(gru_status_t *status)
{
    bmic_object_t *ret = gru_alloc(sizeof (bmic_object_t), status);
    gru_alloc_check(ret, NULL);

    ret->type = NULL_TYPE;

    return ret;
}

void bmic_object_destroy(bmic_object_t **ptr)
{
    bmic_object_t *obj = *ptr;

    if (!obj) {
        return;
    }

    if (obj->type == STRING) {
        if (obj->data.str) {
            free(obj->data.str);
        }
    }

    gru_dealloc((bmic_object_t **) ptr);
}

void bmic_object_set_string(bmic_object_t *obj, const char *value)
{
    obj->type = STRING;
    asprintf(&obj->data.str, "%s", value);
}

void bmic_object_set_integer(bmic_object_t *obj, int64_t value)
{
    obj->type = INTEGER;
    obj->data.number = value;
}

void bmic_object_set_boolean(bmic_object_t *obj, bool value)
{
    obj->type = BOOLEAN;
    obj->data.value = value;
}

void bmic_object_set_double(bmic_object_t *obj, double value)
{
    obj->type = DOUBLE;
    obj->data.d = value;
}

void bmic_object_set_null(bmic_object_t *obj) {
    obj->type = NULL_TYPE;
    obj->data.str = NULL;
}