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

#ifndef BMIC_COMMON_CLI_HEADER_H
#define BMIC_COMMON_CLI_HEADER_H

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <cli/gru_cli_opt.h>
#include <common/gru_colors.h>

#ifndef HAVE_STRLCPY
#include <string/gru_alt.h>
#endif // HAVE_STRLCPY

#include "base/common/bmic_cap_info.h"
#include "base/common/bmic_list.h"
#include "base/format/bmic_json.h"
#include "base/transport/bmic_endpoint.h"
#include "base/transport/bmic_endpoint_http.h"

#include "context/bmic_context.h"

#include "base/common/bmic_regex.h"

#include "management/common/bmic_complements_java.h"
#include "management/common/bmic_queue_stat.h"

#include "bmic/util/bmic_action_common.h"
#include "bmic/util/bmic_opt_common.h"
#include "bmic/util/bmic_print_simple.h"
#include "bmic/util/bmic_type_map.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* BMIC_COMMON_CLI_HEADER_H */
