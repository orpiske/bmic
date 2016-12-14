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

static bool log_initialized = false;

static void bmic_logger_default_printer(log_level_t level, const char *msg, ...) {
    if (!gru_logger_can_log(level)) {
        return;
    }

    va_list ap;
    va_start(ap, msg);
    gru_logger_default_do_print(level, msg, ap);
    va_end(ap);
}

void bmic_log_initialization() {
    if (log_initialized) {
        return;
    }
    
    const char *log_level_str = getenv("BMIC_DEBUG");
    log_level_t log_level;
    if (!log_level_str) {
        log_level = FATAL;
    }
    else {
        log_level = gru_logger_get_level(log_level_str);
    }
    
    gru_logger_set_mininum(log_level);
    gru_logger_set(bmic_logger_default_printer);
    
    log_initialized = true;

}

bool bmic_context_init_simple(bmic_context_t *ctxt, const char *server, 
                                        const char *username, 
                                        const char *password,                               
                                        gru_status_t *status) {
    
    bmic_log_initialization();
        
    bmic_discovery_hint_t *hint = bmic_discovery_hint_eval_addressing(server, BMIC_PORT_UNKNOWN,
                                               status);
    if (!hint) {
        return false;
    }
    
    bool ret = bmic_context_init_hint(ctxt, hint, username, password, status);
    if (!ret) {
        bmic_discovery_hint_destroy(&hint);
    }
    
    return ret;
}


bool bmic_context_init_hint(bmic_context_t *ctxt, bmic_discovery_hint_t *hint, 
                                        const char *username, 
                                        const char *password,                               
                                        gru_status_t *status) {
    
    bmic_log_initialization();
    
    if (hint == NULL) {
        return false;
    }
    
    bmic_product_registry_init(status);
    bmic_product_register(status);
    
    ctxt->hint = hint;
    
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