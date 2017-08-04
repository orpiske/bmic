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
#include "bmic_op_info.h"

bmic_op_info_t *bmic_op_info_new(gru_status_t *status) {
	bmic_op_info_t *ret = gru_alloc(sizeof(bmic_op_info_t), status);
	gru_alloc_check(ret, NULL);

	ret->signature = gru_list_new(status);
	if (!ret->signature) {
		gru_dealloc((void **) &ret);

		return NULL;
	}

	return ret;
}

static void bmic_op_info_destroy_sigs(const void *nodedata, void *payload) {
	bmic_op_sig_destroy((bmic_op_sig_t **) &nodedata);
}

void bmic_op_info_destroy(bmic_op_info_t **ptr) {
	bmic_op_info_t *obj = *ptr;

	if (obj == NULL) {
		return;
	}

	if (obj->name != NULL) {
		gru_dealloc_string(&obj->name);
	}

	if (obj->signature) {
		gru_list_for_each(obj->signature, bmic_op_info_destroy_sigs, NULL);
	}

	gru_list_destroy(&obj->signature);

	gru_dealloc((void **) ptr);
}

inline void bmic_op_info_destroy_list(void **ptr) {
	bmic_op_info_destroy((bmic_op_info_t **) ptr);
}

void bmic_op_info_set_name(bmic_op_info_t *op, const char *name) {
	assert(op != NULL);

	if (asprintf(&op->name, "%s", name) == -1) {
		logger_t logger = gru_logger_get();
		logger(GRU_FATAL, "Unable to allocate memory for saving the operation name\n");
	}
}

void bmic_op_info_add_signature(bmic_op_info_t *op, const bmic_op_sig_t *sig) {
	assert(op != NULL);
	assert(op->signature != NULL);

	if (gru_list_append(op->signature, sig) == NULL) {
		logger_t logger = gru_logger_get();

		logger(GRU_FATAL, "Unable to add new argument\n");
	}
}