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
#ifndef BMIC_ACTIVEMQ_JSON_H
#define BMIC_ACTIVEMQ_JSON_H

#include <common/gru_alloc.h>
#include <common/gru_status.h>

#include "base/format/bmic_json.h"
#include "bmic_activemq_mi.h"

#define ACTIVEMQ_CREATE_QUEUE_SIG "addQueue(java.lang.String)"
#define ACTIVEMQ_REMOVE_QUEUE_SIG "removeQueue(java.lang.String)"
#define ACTIVEMQ_PURGE_QUEUE_SIG "purge()"
#define ACTIVEMQ_RESET_QUEUE_SIG "resetStatistics()"

#ifdef __cplusplus
extern "C" {
#endif

void bmic_activemq_json_create_queue(
	const bmic_object_t *op, bmic_json_t *json, const char *name);

void bmic_activemq_json_delete_queue(
	const bmic_object_t *op, bmic_json_t *json, const char *name);

void bmic_activemq_json_purge_queue(const bmic_object_t *op, bmic_json_t *json);

void bmic_activemq_json_reset_queue(const bmic_object_t *op, bmic_json_t *json);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_ACTIVEMQ_JSON_H */
