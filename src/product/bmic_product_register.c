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
#include "bmic_product_register.h"

void bmic_product_register(gru_status_t *status) {
	logger_t logger = gru_logger_get();

	bmic_api_interface_t *artemis = bmic_artemis_product(status);
	if (artemis) {
		bmic_product_registry_add(artemis, status);

		if (gru_status_error(status)) {
			logger(GRU_ERROR, "Unable to register Artemis management API interface");
			return;
		}
	} else {
		logger(GRU_ERROR, "Unable to create a new Artemis management API interface");
	}

	bmic_api_interface_t *activemq = bmic_activemq_product(status);
	if (activemq) {
		bmic_product_registry_add(activemq, status);
		if (gru_status_error(status)) {
			logger(GRU_ERROR, "Unable to register Artemis management API interface");
		}
	} else {
		logger(GRU_ERROR, "Unable to create a new ActiveMQ management API interface");
	}

	bmic_api_interface_t *jamq6 = bmic_jamq6_product(status);
	if (jamq6) {
		bmic_product_registry_add(jamq6, status);
		if (gru_status_error(status)) {
			logger(GRU_ERROR, "Unable to register JBoss A-MQ 6 management API interface");
		}
	} else {
		logger(GRU_ERROR, "Unable to create a new JBoss A-MQ 6 management API interface");
	}
}

static void bmic_product_unregister_ex(const void *nodedata, void *data) {
	bmic_api_interface_t *product = (bmic_api_interface_t *) nodedata;

	logger_t logger = gru_logger_get();

	logger(GRU_DEBUG, "Unregistering: %s", product->name);
	bmic_api_interface_destroy(&product);
}

void bmic_product_unregister() {
	const gru_list_t *registry = bmic_product_registry();

	gru_list_for_each(registry, bmic_product_unregister_ex, NULL);
}
