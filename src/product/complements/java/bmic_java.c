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
#include "bmic_java.h"


const bmic_exchange_t *bmic_java_attribute_read(bmic_handle_t *handle,
                                                   const bmic_exchange_t *cap,
                                                   const char *name,
                                                   gru_status_t *status)
{
    return bmic_artemis_mi_read(handle, cap->root, name, status,
                             REG_SEARCH_NAME, JAVA_CAPABILITIES_KEY_REGEX);
}