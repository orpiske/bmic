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
#ifndef BMIC_CONTEXT_H
#define BMIC_CONTEXT_H

#include <stdbool.h>
#include <stdlib.h>

#include <log/gru_logger.h>

#include "base/common/bmic_credentials.h"

#include "management/common/bmic_discovery_hint.h"
#include "management/common/bmic_handle.h"
#include "management/common/bmic_product.h"

#include "product/bmic_product_register.h"

typedef struct bmic_context_t_ {
	bmic_discovery_hint_t *hint;
	bmic_credentials_t *credentials;
	bmic_handle_t *handle;
	bmic_api_interface_t *api;
} bmic_context_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes logging
 */
void bmic_log_initialization();

/**
 * Initializes the BMIC API context
 * @param ctxt the context
 * @param server the server hostname/ip
 * @param username username if required
 * @param password password if required
 * @param status status object
 * @return true if success or false otherwise (error details will be stored in the status
 * object)
 */
bool bmic_context_init_simple(bmic_context_t *ctxt, const char *server,
	const char *username, const char *password, gru_status_t *status);

bool bmic_context_init_hint(bmic_context_t *ctxt, bmic_discovery_hint_t *hint,
	const char *username, const char *password, gru_status_t *status);
/**
 * Destroys the BMIC API context
 * @param context the context to destroy
 */
void bmic_context_cleanup(bmic_context_t *context);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_CONTEXT_H */
