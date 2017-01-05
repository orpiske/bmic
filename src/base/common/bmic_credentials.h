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
#ifndef BMIC_CREDENTIALS_H
#define BMIC_CREDENTIALS_H

#include <assert.h>

#include <common/gru_alloc.h>
#include <common/gru_status.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bmic_credentials_t_ {
	char *username;
	char *password;
} bmic_credentials_t;

bmic_credentials_t bmic_credentials_init(
	const char *username, const char *password, gru_status_t *status);

bmic_credentials_t bmic_credentials_clone(bmic_credentials_t, gru_status_t *status);

void bmic_credentials_cleanup(bmic_credentials_t *credentials);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_CREDENTIALS_H */
