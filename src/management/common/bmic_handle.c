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
#include "bmic_handle.h"

void bmic_handle_destroy(bmic_handle_t **handle, bmic_handle_ep_cleanup_fn cleanup) {
	bmic_handle_t *h = *handle;

	if (!h) {
		return;
	}

	if (h->ep) {
		gru_status_t status = gru_status_new();

		cleanup(h->ep, &status);
		if (gru_status_error(&status)) {
			logger_t logger = gru_logger_get();

			logger(GRU_WARNING, "Failed to cleanup CURL handle: %s\n", status.message);
		}

		bmic_endpoint_destroy(&h->ep);
	}

	gru_dealloc((void **) handle);
}
