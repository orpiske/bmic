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
#include "bmic_context.h"
#include "management/common/bmic_product.h"

bool bmic_context_init_simple(bmic_context_t *ctxt, const char *server, 
                                        const char *username, 
                                        const char *password,                               
                                        gru_status_t *status) {
    bmic_product_registry_init(status);
    bmic_product_register(status);
    
    ctxt->hint = bmic_discovery_hint_eval_addressing(server, BMIC_PORT_UNKNOWN,
                                               status);
    if (!ctxt->hint) {
        return false;
    }
    
    if (username && password) { 
        ctxt->credentials = bmic_credentials_init(username, password, status);
        if (!ctxt->credentials) {
            bmic_context_cleanup(ctxt);

            return false;
        }
    }

    ctxt->api = bmic_discovery_run(ctxt->hint, ctxt->credentials, &ctxt->handle, status);
    if (!ctxt->api) {
        bmic_context_cleanup(ctxt);
        
        return false;
    }
        
    return true;
}


void bmic_context_cleanup(bmic_context_t *ctxt) {
    if (!ctxt) {
        return;
    }
    
    if (ctxt->api) {
        ctxt->api->api_cleanup(&ctxt->handle);
    }

    // These will unregister the product and will cleanup memory pointed byctxt->api 
    // as well
    bmic_product_unregister();
    bmic_product_registry_destroy();
    
    bmic_discovery_hint_destroy(&ctxt->hint);
    bmic_credentials_detroy(&ctxt->credentials);
}