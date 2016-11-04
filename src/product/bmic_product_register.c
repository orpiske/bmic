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
    bmic_api_interface_t *artemis = bmic_artemis_product(status);
    if (!artemis) {
        return;
    }

    bmic_product_registry_add(artemis, status);
    if (status->code != GRU_SUCCESS) {
        return;
    }

    bmic_api_interface_t *activemq = bmic_activemq_product(status);
    if (!activemq) {
        return;
    }

    bmic_product_registry_add(activemq, status);
    if (status->code != GRU_SUCCESS) {
        return;
    }
}

static void bmic_product_unregister_ex(const gru_node_t *node, void *data) {
    bmic_api_interface_t *product = gru_node_get_data_ptr(bmic_api_interface_t, node);

    // printf("Unregistering: %s\n", product->name);
    bmic_product_destroy(&product);
}


void bmic_product_unregister() {
    const gru_list_t *registry = bmic_product_registry();

    gru_list_for_each(registry, bmic_product_unregister_ex, NULL);
}
