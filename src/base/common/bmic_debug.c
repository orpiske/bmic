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
#include "bmic_debug.h"


void bmic_debug_do_print(const char *format, ...) {
#ifndef BMIC_DEBUG
    if (getenv("BMIC_DEBUG")) {
            printf("DEBUG: ");
        
            va_list ap;

            va_start(ap, format);

            vprintf(format, ap);

            va_end(ap);
        
        
    }
#else 
    printf("DEBUG: ");
    
    va_list ap;

    va_start(ap, format);

    vprintf(format, ap);

    va_end(ap);
#endif // BMIC_DEBUG
}


void bmic_debug_do_print_verbose(const char *file, const char *func, 
                                 int linenum, const char *format, ...) {
#ifndef BMIC_DEBUG
    if (!getenv("BMIC_DEBUG")) {
        return;
    }
#else 
    printf("[DEBUG %s@%s:%d] ", func, file, linenum);
    
    va_list ap;

    va_start(ap, format);

    vprintf(format, ap);

    va_end(ap);
#endif // BMIC_DEBUG
}