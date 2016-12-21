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

#ifndef BMIC_EXCHANGE_H
#define BMIC_EXCHANGE_H

#include "base/common/bmic_cap_info.h"
#include "base/common/bmic_object.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum bmic_exchange_type_t_ {
	EX_CAP_LIST,
	EX_CAP_ENTRY,
} bmic_exchange_type_t;

typedef union bmic_exchange_payload_t_ {
	const bmic_cap_info_t *capinfo;
} bmic_exchange_payload_t;

typedef struct bmic_exchange_t_ {
	bmic_exchange_type_t type;
	const bmic_object_t *root;
	const bmic_object_t *data_ptr;
	bmic_exchange_payload_t payload;
} bmic_exchange_t;

#ifdef __cplusplus
}
#endif

#endif /* BMIC_EXCHANGE_H */
