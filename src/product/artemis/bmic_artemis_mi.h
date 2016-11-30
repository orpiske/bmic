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
#ifndef BMIC_ARTEMIS_MI_H
#define BMIC_ARTEMIS_MI_H

#include <common/gru_status.h>
#include <common/gru_alloc.h>

#include "base/common/bmic_credentials.h"
#include "base/common/bmic_cap_info.h"
#include "base/common/bmic_op_arg.h"
#include "base/common/bmic_op_sig.h"
#include "base/common/bmic_op_info.h"
#include "base/transport/bmic_transport.h"
#include "base/transport/bmic_endpoint_http.h"
#include "base/format/bmic_json.h"

#include "management/common/bmic_handle.h"
#include "management/discovery/bmic_discovery.h"

#include "product/common/bmic_api_io.h"
#include "product/common/bmic_internal_payloads.h"
#include "product/common/bmic_path.h"
#include "product/common/bmic_finder.h"

#define ARTEMIS_READ "read"
#define ARTEMIS_BASE_PKG "org.apache.activemq.artemis"
#define ARTEMIS_STATUS_OK 200

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Reads an attribute from the management interface
 * @param handle
 * @param root
 * @param attr_name
 * @param status
 * @param flags
 * @param regex_fmt
 * @return 
 */
const bmic_exchange_t *bmic_artemis_mi_read(bmic_handle_t *handle,
                                                const bmic_object_t *root,
                                                const char *attr_name,
                                                gru_status_t *status,
                                                int flags,
                                                const char *regex_fmt,
                                                ...);
    
/**
 * Given a node of attributes, read them into the info object
 * @param obj
 * @param info
 */
void bmic_artemis_mi_translate_attr(const bmic_object_t *obj,
                                         bmic_cap_info_t *info);


/**
 * Given a node of operations, read them into the info object
 * @param obj
 * @param info
 */
void bmic_artemis_mi_translate_op(const bmic_object_t *obj,
                                         bmic_op_info_t *info,
                                        gru_status_t *status);


bool bmic_artemis_mi_translate_status(bmic_object_t *root, gru_status_t *status);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_ARTEMIS_MI_H */

