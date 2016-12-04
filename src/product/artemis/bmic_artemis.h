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
#ifndef BMIC_ARTEMIS_H
#define BMIC_ARTEMIS_H

#include <common/gru_status.h>
#include <common/gru_alloc.h>

#include "base/common/bmic_credentials.h"
#include "base/common/bmic_cap_info.h"
#include "base/common/bmic_op_arg.h"
#include "base/common/bmic_op_sig.h"
#include "base/common/bmic_op_info.h"
#include "base/common/bmic_data.h"
#include "base/transport/bmic_transport.h"
#include "base/transport/bmic_endpoint_http.h"
#include "base/format/bmic_json.h"

#include "management/common/bmic_handle.h"
#include "management/common/bmic_api_io.h"
#include "management/common/bmic_internal_payloads.h"
#include "management/discovery/bmic_discovery.h"
#include "management/interfaces/jolokia/bmic_jolokia.h"
#include "management/interfaces/jolokia/bmic_jolokia_io.h"

#include "product/common/bmic_path.h"
#include "product/common/bmic_finder.h"

#include "bmic_artemis_urls.h"
#include "bmic_artemis_mi.h"
#include "bmic_artemis_json.h"

#ifdef __cplusplus
extern "C" {
#endif

bmic_api_interface_t *bmic_artemis_product(gru_status_t *status);
const char *bmic_artemis_base_url(const bmic_discovery_hint_t *hint);
bmic_handle_t *bmic_artemis_init(const char *base_url,
                                 bmic_credentials_t *credentials,
                                 gru_status_t *status);
void bmic_artemis_cleanup(bmic_handle_t **handle);

bmic_product_info_t *bmic_artemis_product_info(bmic_handle_t *handle,
        gru_status_t *status);

/**
 * Load all available management/attribute capabilites
 * @param handle 
 * @param status
 */
const bmic_exchange_t *bmic_artemis_load_capabilities(bmic_handle_t *handle,
        gru_status_t *status);

/**
 * Read a single an attribute from the MI
 * @param handle
 * @param capabilities
 * @param name
 * @param status
 * @return 
 */
const bmic_exchange_t *bmic_artemis_attribute_read(bmic_handle_t *handle,
        const bmic_exchange_t *capabilities, const char *name,
        gru_status_t *status);

/**
 * Get a list of all available broker attributes and their metadata
 * @param handle
 * @param cap
 * @param status
 * @return 
 */
const bmic_list_t *bmic_artemis_attribute_list(bmic_handle_t *handle,
        const bmic_exchange_t *cap, gru_status_t *status);

/**
 * Read a single an attribute from the MI
 * @param handle
 * @param capabilities
 * @param name
 * @param status
 * @return 
 */
const bmic_exchange_t *bmic_artemis_queue_attribute_read(bmic_handle_t *handle,
        const bmic_exchange_t *capabilities, const char *name,
        gru_status_t *status, const char *queue);



/**
 * Get a list of all available broker operations and their metadata
 * @param handle
 * @param cap
 * @param status
 * @return 
 */
const bmic_list_t *bmic_artemis_operation_list(bmic_handle_t *handle,
                                              const bmic_exchange_t *cap, 
                                                gru_status_t *status);

/**
 * Create a core queue
 * @param handle
 * @param cap
 * @param name
 * @param status
 * @return 
 */
bool bmic_artemis_operation_create_queue(bmic_handle_t *handle,
                                            const bmic_exchange_t *cap, 
                                            const char *name,
                                            gru_status_t *status);


/**
 * Deletes a core queue
 * @param handle
 * @param cap
 * @param name
 * @param status
 * @return 
 */
bool bmic_artemis_operation_delete_queue(bmic_handle_t *handle,
                                            const bmic_exchange_t *cap, 
                                            const char *name,
                                            gru_status_t *status);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_ARTEMIS_H */
