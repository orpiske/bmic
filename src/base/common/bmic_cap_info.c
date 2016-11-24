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
#include "bmic_cap_info.h"

bmic_cap_info_t *bmic_cap_info_new(gru_status_t *status) {
    bmic_cap_info_t *ret = gru_alloc(sizeof(bmic_cap_info_t), status);
    gru_alloc_check(ret, NULL);

    return ret;
}

void bmic_cap_info_destroy(bmic_cap_info_t **ptr) {
    bmic_cap_info_t *obj = *ptr;

    if (obj == NULL) {
        return;
    }

    if (obj->name != NULL) {
        gru_dealloc_string(&obj->name);
    }

    if (obj->description != NULL) {
        gru_dealloc_string(&obj->description);
    }

    if (obj->typename != NULL) {
        gru_dealloc_string(&obj->typename);
    }

    gru_dealloc((void **) ptr);
}

inline void bmic_cap_info_destroy_list(void **ptr) {
    bmic_cap_info_destroy((bmic_cap_info_t **) ptr);
}

void bmic_cap_info_set_name(bmic_cap_info_t *cap, const char *name) {
    assert(cap != NULL);

    if (asprintf(&cap->name, "%s", name) == -1) {
        fprintf(stderr, "Unable to allocate memory for saving the capacity name\n");
    }
}
void bmic_cap_info_set_description(bmic_cap_info_t *cap, const char *description) {
    assert(cap != NULL);

    if (asprintf(&cap->description, "%s", description) == -1) {
        fprintf(stderr, "Unable to allocate memory for saving the capacity description\n");
    }
}

void bmic_cap_info_set_write(bmic_cap_info_t *cap, bool write) {
    assert(cap != NULL);

    cap->write = write;
}


void bmic_cap_info_set_typename(bmic_cap_info_t *cap, const char *typename) {
    assert(cap != NULL);

    if (asprintf(&cap->typename, "%s", typename) == -1) {
        fprintf(stderr, "Unable to allocate memory for saving the capacity typename\n");
    }
}
