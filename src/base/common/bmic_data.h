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
#ifndef BMIC_DATA_H
#define BMIC_DATA_H

#include <assert.h>

#include <common/gru_alloc.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bmic_data_t_ {
	size_t size;
	void *data;
} bmic_data_t;

static inline const char *bmic_data_to_string(const bmic_data_t *data) {
	assert(data != NULL);

	return (const char *) data->data;
}

static inline void bmic_data_release(bmic_data_t *data) {
	if (data && data->data) {
		gru_dealloc(&data->data);
	}
}

#ifdef __cplusplus
}
#endif

#endif /* BMIC_DATA_H */
