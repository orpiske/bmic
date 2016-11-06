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

bool bmic_match(const char *str, const char *regex, gru_status_t *status)
{
    regex_t reg_obj;
    int rc;

    rc = regcomp(&reg_obj, regex, REG_EXTENDED);
    if (rc) {
        gru_status_set(status, GRU_FAILURE, 
                       "Unable to compile the give regex '%s'", regex);
        
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
    gru_status_set(status, GRU_FAILURE, 
                   "Error while trying to match the string");
    return false;
    
}