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

bmic_credentials_t bmic_credentials_init(
	const char *username, const char *password, gru_status_t *status) {
	bmic_credentials_t ret = {0};

	if (username != NULL) {
		if (asprintf(&ret.username, "%s", username) == -1) {
			gru_status_set(
				status, GRU_FAILURE, "Unable to set username: not enough memory");

			bmic_credentials_cleanup(ret);
			return ret;
		}
	}

	if (password != NULL) {
		if (asprintf(&ret.password, "%s", password) == -1) {
			gru_status_set(
				status, GRU_FAILURE, "Unable to set password: not enough memory");

			bmic_credentials_cleanup(ret);

			return ret;
		}
	}

	return ret;
}

bmic_credentials_t bmic_credentials_clone(bmic_credentials_t other, gru_status_t *status) {
	bmic_credentials_t ret = {0};

	if (other.username != NULL) {
		if (asprintf(&ret.username, "%s", other.username) == -1) {
			gru_status_set(
				status, GRU_FAILURE, "Unable to set username: not enough memory");

			bmic_credentials_cleanup(ret);
			return ret;
		}
	}

	if (other.password != NULL) {
		if (asprintf(&ret.password, "%s", other.password) == -1) {
			gru_status_set(
				status, GRU_FAILURE, "Unable to set password: not enough memory");

			bmic_credentials_cleanup(ret);

			return ret;
		}
	}

	return ret;
}

void bmic_credentials_cleanup(bmic_credentials_t credentials) {
	if (credentials.username) {
		gru_dealloc_string(&credentials.username);
	}

	if (credentials.password) {
		gru_dealloc_string(&credentials.password);
	}
}