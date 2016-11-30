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
#ifndef BMIC_CAP_INFO_H
#define BMIC_CAP_INFO_H

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include <common/gru_alloc.h>
#include <common/gru_status.h>

#include <log/gru_logger.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bmic_cap_info_t_ {
    char *name;
    char *description;
    bool write;
    char *typename;
} bmic_cap_info_t;

bmic_cap_info_t *bmic_cap_info_new(gru_status_t *status);
void bmic_cap_info_destroy(bmic_cap_info_t **ptr);
void bmic_cap_info_destroy_list(void **ptr);

void bmic_cap_info_set_name(bmic_cap_info_t *cap, const char *name);
void bmic_cap_info_set_description(bmic_cap_info_t *cap, const char *description);
void bmic_cap_info_set_write(bmic_cap_info_t *cap, bool write);
void bmic_cap_info_set_typename(bmic_cap_info_t *cap, const char *typename);



#ifdef __cplusplus
}
#endif

#endif /* BMIC_CAP_INFO_H */

