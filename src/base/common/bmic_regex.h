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

#ifndef BMIC_REGEX_H
#define BMIC_REGEX_H

#include <regex.h>
#include <stdbool.h>

#include <common/gru_status.h>
#include <log/gru_logger.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Matches a string with a regex
 * @param str the string to match
 * @param regex the regex
 * @param status A status object
 * @return true if it matches or false otherwise. Calling functions should check the
 * status object if the return is false for checking errors
 */
bool bmic_match(const char *str, const char *regex, gru_status_t *);

/**
 * Finds a string and returns the match
 * @param str the string to match
 * @param regex the regex
 * @param size An expected size for the matching expressions (should be bigger than 0)
 * @param group The group to match
 * @param status A status object
 * @return A pointer to the matching regex or NULL if not found. The string must be free'd
 * after use
 */
const char *bmic_regex_find(const char *str, const char *regex, uint32_t size,
	uint32_t group, gru_status_t *status);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_REGEX_H */
