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
#ifndef BMIC_ACTIVEMQ_MI_H
#define BMIC_ACTIVEMQ_MI_H

#include <common/gru_alloc.h>
#include <common/gru_status.h>

#include "base/common/bmic_cap_info.h"
#include "base/common/bmic_credentials.h"
#include "base/common/bmic_op_arg.h"
#include "base/common/bmic_op_info.h"
#include "base/common/bmic_op_sig.h"
#include "base/format/bmic_json.h"
#include "base/transport/bmic_endpoint_http.h"
#include "base/transport/bmic_transport.h"

#include "management/common/bmic_handle.h"
#include "management/discovery/bmic_discovery.h"
#include "management/interfaces/jolokia/bmic_jolokia.h"
#include "management/interfaces/jolokia/bmic_jolokia_io.h"
#include "management/interfaces/jolokia/bmic_jolokia_parse.h"

#include "product/common/bmic_finder.h"
#include "product/common/bmic_path.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ACTIVEMQ_READ "read"
#define ACTIVEMQ_BASE_PKG "org.apache.activemq"

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
const bmic_exchange_t *bmic_activemq_mi_read(bmic_handle_t *handle,
	const bmic_object_t *root, const char *attr_name, gru_status_t *status, int flags,
	const char *regex_fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_ACTIVEMQ_MI_H */
