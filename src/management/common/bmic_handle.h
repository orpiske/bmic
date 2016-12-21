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
#ifndef BMIC_HANDLE_H
#define BMIC_HANDLE_H

#include <log/gru_logger.h>

#include "base/transport/bmic_transport.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Defines a cleanup function for releasing resources allocated for the ep
 */
typedef void (*bmic_handle_ep_cleanup_fn)(bmic_endpoint_t *ep, gru_status_t *status);

/**
 * Defines a formatter function to format paths (if needed) required to access
 * the management resources
 * @param op Operation (ie.: read, write, list, etc)
 * @param interface The management interface to set in the path
 * @param pkg Root package
 * @param capname Capability name
 * @param status status
 */
typedef const char *(*bmic_handle_path_formatter_fn)(const char *op,
	const char *interface, const char *pkg, const char *name, gru_status_t *status);

typedef struct bmic_handle_t_ {
	bmic_endpoint_t *ep;
	bmic_transport_t transport;
	/**
	 * Optional path formatter. If not give, should be ignored
	 */
	bmic_handle_path_formatter_fn path_formatter;
} bmic_handle_t;

void bmic_handle_destroy(bmic_handle_t **handle, bmic_handle_ep_cleanup_fn cleanup);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_HANDLE_H */
