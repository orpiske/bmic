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
#ifndef BMIC_PRODUCT_INFO_H
#define BMIC_PRODUCT_INFO_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bmic_product_info_t_ {
	char name[64];
	char version[16];
} bmic_product_info_t;

static inline void bmic_product_info_cleanup(bmic_product_info_t **ptr) {
	gru_dealloc((void **) ptr);
}

#ifdef __cplusplus
}
#endif

#endif /* BMIC_PRODUCT_INFO_H */
