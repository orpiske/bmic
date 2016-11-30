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
#include "bmic_type_map.h"

static const char typemap[][30] = {
    "string",
    "object"
};

inline const char *bmic_type_map(const char *other) {
    if (strstr(other, "String")) {
        return typemap[0];
    }
    else {
        if (strstr(other, "Object")) {
            return typemap[1];
        }
    }

    return other;
}