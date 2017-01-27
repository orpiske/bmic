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
#ifndef BMIC_FINDER_H
#define BMIC_FINDER_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <common/gru_status.h>

#include "base/common/bmic_object.h"
#include "base/common/bmic_regex.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Given an hierarchy (tree) of objects it navigates through it and tries
 * to find the node that matches the given regex
 * @param root The root object
 * @param regex_fmt The regex to match
 * @param flags The search flags as those used by bmic_regex
 * @param status The status object
 * @param ap A previously initialized va_list
 * @return The first object that matches the regex or NULL if not found (status
 * will be properly set in this case)
 */
const bmic_object_t *bmic_finder_varg(const bmic_object_t *root, const char *regex_fmt,
	int flags, gru_status_t *status, va_list ap);

/**
 * Given an hierarchy (tree) of objects it navigates through it and tries to find the
 * node that matches the given regex
 * @param root The root object
 * @param regex_fmt The regex to match
 * @param flags The search flags as those used by bmic_regex
 * @param status The status object
 * @param ...
 * @return The first object that matches the regex or NULL if not found (status
 * will be properly set in this case)
 */
const bmic_object_t *bmic_finder_simple(const bmic_object_t *root, gru_status_t *status,
	int flags, const char *regex_fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_FINDER_H */
