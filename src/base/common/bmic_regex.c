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
#include "bmic_regex.h"

bool bmic_match(const char *str, const char *regex, gru_status_t *status) {
	regex_t reg_obj;
	int rc;

	rc = regcomp(&reg_obj, regex, REG_EXTENDED);
	if (rc) {
		gru_status_set(
			status, GRU_FAILURE, "Unable to compile the give regex '%s'", regex);

		return false;
	}

	rc = regexec(&reg_obj, str, 0, NULL, 0);
	if (rc == 0) {
		regfree(&reg_obj);

		return true;
	}

	regfree(&reg_obj);
	if (rc == REG_NOMATCH) {
		return false;
	}

	// TODO: check the result from regerror
	gru_status_set(status, GRU_FAILURE, "Error while trying to match the string");
	return false;
}

const char *bmic_regex_find(const char *str, const char *regex, uint32_t size,
	uint32_t group, gru_status_t *status) {
	regex_t reg_obj;
	int rc;

	rc = regcomp(&reg_obj, regex, REG_EXTENDED);
	if (rc) {
		gru_status_set(
			status, GRU_FAILURE, "Unable to compile the give regex '%s'", regex);

		return NULL;
	}

	regmatch_t *match = calloc(size, sizeof(regmatch_t));
	if (!match) {
		gru_status_set(status, GRU_FAILURE, "Unable to allocate memory for match object");

		return NULL;
	}

	rc = regexec(&reg_obj, str, size, match, 0);
	if (rc == 0 && ((match + group)->rm_so != -1)) {
		logger_t logger = gru_logger_get();

		logger(GRU_TRACE, "First offset: %d\n", (match + group)->rm_so);
		logger(GRU_TRACE, "Last offset: %d\n", (match + group)->rm_eo);

		int str_size = ((int) (match + group)->rm_eo - (match + group)->rm_so) + 1;
		char *ret = calloc(1, (size_t) str_size + 1);
		if (!ret) {
			gru_status_set(
				status, GRU_FAILURE, "Not enough memory for saving the regex match");

			regfree(&reg_obj);
			free(match);

			return NULL;
		}

		snprintf(ret, (size_t) str_size, "%s", str + (match + group)->rm_so);
		regfree(&reg_obj);
		free(match);

		return ret;
	}

	free(match);
	regfree(&reg_obj);
	if (rc == REG_NOMATCH) {
		return NULL;
	}

	// TODO: check the result from regerror
	gru_status_set(status, GRU_FAILURE, "Error while trying to match the string");
	return NULL;
}