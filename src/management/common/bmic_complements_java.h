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
#ifndef BMIC_COMPLEMENTS_JAVA_H
#define BMIC_COMPLEMENTS_JAVA_H

#include <stdint.h>

#include <common/gru_alloc.h>
#include <common/gru_status.h>

#include "bmic_handle.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bmic_java_mem_info_t_ {
	int64_t init;
	int64_t committed;
	int64_t max;
	int64_t used;
} bmic_java_mem_info_t;

typedef enum bmic_java_memory_model_t_ {
	/**
	 * Memory model for Java 6 and 7 (ie.: no metaspace)
	 */
	BMIC_JAVA_LEGACY,

	/**
	 * Memory model for Java 8
	 */
	BMIC_JAVA_MODERN,
} bmic_java_memory_model_t;

typedef struct bmic_java_info_t_ {
	const char *name;
	const char *version;
	const char *jvm_package_version;
	bmic_java_memory_model_t memory_model;
} bmic_java_info_t;

static inline void bmic_java_info_cleanup(bmic_java_info_t info) {
	gru_dealloc_string((char **) &info.name);
	gru_dealloc_string((char **) &info.version);
	gru_dealloc_string((char **) &info.jvm_package_version);
};

typedef struct bmic_java_os_info_t_ {
	const char *name;
	const char *arch;
	const char *version;

	int64_t mem_total;
	int64_t mem_free;

	int64_t swap_total;
	int64_t swap_free;
	int64_t swap_committed;

	double load_average;
	double process_cpu_load;
	double system_cpu_load;
	int64_t process_cpu_time;

	int64_t open_fd;
	int64_t max_fd;

	int64_t cpus;
} bmic_java_os_info_t;

static inline void bmic_java_os_info_cleanup(bmic_java_os_info_t info) {
	gru_dealloc_string((char **) &info.name);
	gru_dealloc_string((char **) &info.arch);
	gru_dealloc_string((char **) &info.version);
}

typedef bmic_java_info_t (*bmic_complement_api_java_info_fn)(
	bmic_handle_t *handle, gru_status_t *status);

typedef bmic_java_mem_info_t (*bmic_complement_api_java_mem_info_fn)(
	bmic_handle_t *handle, gru_status_t *status);

typedef bmic_java_os_info_t (*bmic_complement_api_java_os_info_fn)(
	bmic_handle_t *handle, gru_status_t *status);

typedef struct bmic_complements_java_api_t_ {
	bmic_complement_api_java_info_fn java_info;
	bmic_complement_api_java_mem_info_fn eden_info;
	bmic_complement_api_java_mem_info_fn survivor_info;
	// aka PS Old Gen
	bmic_complement_api_java_mem_info_fn tenured_info;

	bmic_complement_api_java_mem_info_fn code_cache_info;

	// Only if memory model == JAVA_MODERN
	bmic_complement_api_java_mem_info_fn metaspace_info;

	// Only if memory model == JAVA_LEGACY
	bmic_complement_api_java_mem_info_fn permgen_info;

	bmic_complement_api_java_os_info_fn os_info;

} bmic_complements_java_api_t;

#ifdef __cplusplus
}
#endif

#endif /* BMIC_COMPLEMENTS_JAVA_H */
