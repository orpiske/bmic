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
#include "bmic_path.h"

const char *bmic_path_formatter(const char *op, const char *interface, const char *pkg,
	const char *capname, gru_status_t *status) {
	char *ret;

	int rc = asprintf(&ret, "%s/%s:%s/%s", op, pkg, interface, capname);

	if (rc == -1) {
		gru_status_set(
			status, GRU_FAILURE, "Not enough memory to format capabilities path");

		return NULL;
	}

	return ret;
}
