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
#include "bmic_list.h"


inline bmic_list_t *bmic_list_new(gru_status_t *status, node_destructor destructor) {
    bmic_list_t *ret = gru_alloc(sizeof(bmic_list_t), status);
    gru_alloc_check(ret, NULL);
    
    ret->items = gru_list_new(status);
    if (!ret->items) {
        gru_dealloc(&ret);
        
        return NULL;
    }
    
    ret->destructor = destructor;
    return ret;
}

static void bmic_caplist_node_destroy(const void *nodedata, void *payload)
{
    bmic_cap_info_t *info = (bmic_cap_info_t *) nodedata;
    node_destructor destructor = (node_destructor) payload;
    
    destructor(&info);
}

void bmic_list_destroy(bmic_list_t **ptr) {
    bmic_list_t *list = (bmic_list_t *) *ptr;
    
    gru_list_for_each(list->items, bmic_caplist_node_destroy, list->destructor);
    gru_list_destroy(&list->items);
    gru_dealloc((void **)ptr);
}
