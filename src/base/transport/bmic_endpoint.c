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
#include "bmic_endpoint.h"


bmic_endpoint_t *bmic_endpoint_init(const char *url, const char *username,
        const char *password, gru_status_t *status)
{
    assert(url != NULL);
    
    bmic_endpoint_t *ret = gru_alloc(sizeof(bmic_endpoint_t), status);
    gru_alloc_check(ret, NULL);

    asprintf(&ret->url, "%s", url);

    ret->credentials = bmic_credentials_init(username, password, status);
    return ret;
}


void bmic_endpoint_destroy(bmic_endpoint_t **ep) {
    bmic_endpoint_t *e = *ep;

    if (e->credentials) {
        bmic_credentials_detroy(&e->credentials);
    }

    if (e->path) {
        gru_dealloc_string(&e->path);
    }
    
    if (e->url) {
        gru_dealloc_string(&e->url);
    }

    gru_dealloc((void **) ep);
}

void bmic_endpoint_set_credentials(bmic_endpoint_t *ep,
    const bmic_credentials_t *credentials, gru_status_t *status)
{
    if (!ep) {
        gru_status_set(status, GRU_FAILURE, "Invalid endpoint (null)");

        return;
    }

    if (ep->credentials) {
        bmic_credentials_detroy(&ep->credentials);
    }

    ep->credentials = bmic_credentials_clone(credentials, status);
}


void bmic_endpoint_set_path(bmic_endpoint_t *ep, const char *path)
{
    assert(ep != NULL);
    
    if (ep->path != NULL) {
        gru_dealloc_string(&ep->path);
    }

    if (path == NULL) {
        ep->path = NULL;

        return;
    }

    asprintf(&ep->path, "%s", path);
}

void bmic_endpoint_reset_path(bmic_endpoint_t *ep) {
    bmic_endpoint_set_path(ep, NULL);
}
