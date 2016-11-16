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
#include "bmic_caplist.h"


inline bmic_caplist_t *bmic_caplist_new(gru_status_t *status) {
    return gru_list_new(status);
}

static void bmic_caplist_node_destroy(const void *nodedata, void *payload)
{
    bmic_cap_info_t *info = (bmic_cap_info_t *) nodedata;
    
    bmic_cap_info_destroy(&info);
}

void bmic_caplist_destroy(bmic_caplist_t **ptr) {
    bmic_caplist_t *caplist = (bmic_caplist_t *) *ptr;
    
    gru_list_for_each(caplist, bmic_caplist_node_destroy, NULL);
    gru_list_destroy(ptr);
}
