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
#include "bmic_discovery_hint.h"

bmic_discovery_hint_t *bmic_discovery_hint_eval_addressing(const char *hostname, 
                                                           uint16_t port,
                                                           gru_status_t *status)
{
    bmic_discovery_hint_t *ret = gru_alloc(sizeof(bmic_discovery_hint_t), status);
    gru_alloc_check(ret, NULL);
    
    ret->hint_type = ADDRESSING;
    if (hostname == NULL) {
        asprintf(&ret->content.addressing.hostname, "%s", "localhost");
    }
    else {
        asprintf(&ret->content.addressing.hostname, "%s", hostname);
    }
    
    ret->content.addressing.port = port;
    return ret;
}


bmic_discovery_hint_t *bmic_discovery_hint_eval_url(const char *url, 
                                                    gru_status_t *status) {
    bmic_discovery_hint_t *ret = gru_alloc(sizeof(bmic_discovery_hint_t), status);
    gru_alloc_check(ret, NULL);
    
    
    if (url == NULL) {
        return bmic_discovery_hint_eval_addressing(NULL, BMIC_PORT_UNKNOWN, status);
    }
    
    ret->hint_type = URL;
    asprintf(&ret->content.url, "%s", url);
    
    return ret;
}

void bmic_discovery_hint_destroy(bmic_discovery_hint_t **hint) {
    bmic_discovery_hint_t *h = *hint;
    
    if (h == NULL) {
        return;
    }
    
    if (h->hint_type == ADDRESSING) {
        free(h->content.addressing.hostname);
    }
    else {
        if (h->hint_type == URL) {
            free(h->content.url);
        }
    }
    
    gru_dealloc((void **) hint);
}