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
#include "bmic_action_common.h"

void show_info(bmic_api_interface_t *api, bmic_handle_t *handle,
	const bmic_exchange_t *cap, bool show, gru_status_t *status) {

	if (!show) {
		return;
	}

	bmic_product_info_t *info = api->product_info(handle, cap, status);

	if (!info || gru_status_error(status)) {
		fprintf(stderr, "Unable to determine product version: %s\n", status->message);
	}

	if (info) {
		print_product_info(api, info);

		gru_dealloc((void **) &info);
	}
}