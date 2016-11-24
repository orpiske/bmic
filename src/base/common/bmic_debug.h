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
#ifndef BMIC_DEBUG_H
#define BMIC_DEBUG_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
    
void bmic_debug_do_print(const char *format, ...);
void bmic_debug_do_print_verbose(const char *file, const char *func,
        int linenum, const char *format, ...);

/**
 * Debug print wrapper
 */
#define bmic_debug_print(format, ...) bmic_debug_do_print_verbose(strrchr("/" __FILE__, '/') + 1,\
    __func__, __LINE__, format, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* BMIC_DEBUG_H */

