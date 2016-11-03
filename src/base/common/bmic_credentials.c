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
#include "bmic_credentials.h"

bmic_credentials_t *bmic_credentials_init(const char *username, 
                                          const char *password, gru_status_t *status) {
    bmic_credentials_t *ret = gru_alloc(sizeof(bmic_credentials_t), status);
    gru_alloc_check(ret, NULL);
    
    asprintf(&ret->username, "%s", username);
    asprintf(&ret->password, "%s", password);
    
    return ret;
}

bmic_credentials_t *bmic_credentials_clone(bmic_credentials_t *other, gru_status_t *status) {
    bmic_credentials_t *ret = gru_alloc(sizeof(bmic_credentials_t), status);
    gru_alloc_check(ret, NULL);
    
    asprintf(&ret->username, "%s", other->username);
    asprintf(&ret->password, "%s", other->password);
    
    return ret;
}

void bmic_credentials_detroy(bmic_credentials_t **credentials) {
    bmic_credentials_t *c = *credentials;
    
    if (!c) {
        return;
    }
    
    if (c->username) {
        free(c->username);
    }
    
    if (c->password) {
        free(c->password);
    }
   
    gru_dealloc((void **) credentials);
}