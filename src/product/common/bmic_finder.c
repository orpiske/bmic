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
#include "bmic_finder.h"

const bmic_object_t *bmic_finder_varg(const bmic_object_t *root, const char *regex_fmt,
	int flags, gru_status_t *status, va_list ap) {
	char *regex;

	// Build the regex
	int rc = vasprintf(&regex, regex_fmt, ap);
	if (rc == -1) {
		gru_status_set(status, GRU_FAILURE, "Unable to format the matching regex");

		return NULL;
	}

	// ... And uses it to find the matching node in the capability tree
	const bmic_object_t *ptr = bmic_object_find_regex(root, regex, flags);
	if (!ptr) {
#ifdef DNDEBUG
		gru_status_set(
			status, GRU_FAILURE, "Unable to find the objects matching %s", regex);
#else
		gru_status_set(status, GRU_FAILURE, "Unable to find the requested object");
#endif
		free(regex);
		return NULL;
	}

	free(regex);
	return ptr;
}

const bmic_object_t *bmic_finder_simple(const bmic_object_t *root, gru_status_t *status,
	int flags, const char *regex_fmt, ...) {
	va_list ap;
	va_start(ap, regex_fmt);

	const bmic_object_t *ret = bmic_finder_varg(root, regex_fmt, flags, status, ap);
	va_end(ap);

	return ret;
}
