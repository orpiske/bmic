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
#include "bmic_java.h"

static void bmic_java_read_mem_value(const bmic_object_t *root, const char *name,
	const char *path, int64_t *dest, gru_status_t *status) {
	const bmic_object_t *mem = bmic_object_find_by_path(root, path);
	if (!mem) {
		gru_status_set(status, GRU_FAILURE, "Unable to find %s memory value", name);
		return;
	}

	if (mem->type != BMIC_INTEGER) {
		gru_status_set(
			status, GRU_FAILURE, "Invalid data type for %s memory value", name);
		return;
	}

	*dest = mem->data.number;
}

/**
 * Read a single an attribute from the MI
 * @param handle
 * @param capabilities
 * @param name
 * @param status
 * @return
 */
static bmic_java_mem_info_t bmic_java_read_mem_info_any(
	bmic_handle_t *handle, const bmic_object_t *usage, gru_status_t *status) {
	bmic_java_mem_info_t ret = {0};

	if (!usage) {
		return ret;
	}

	bmic_jolokia_translate_status(usage, status);
	if (gru_status_error(status)) {
		return ret;
	}

	const bmic_object_t *init = bmic_object_find_by_path(usage, "/value/init");
	if (!init) {
		gru_status_set(status, GRU_FAILURE, "Unable to find initial memory value");
		return ret;
	}

	if (init->type != BMIC_INTEGER) {
		gru_status_set(status, GRU_FAILURE, "Invalid data type for initial memory value");
		return ret;
	}

	bmic_java_read_mem_value(usage, "init", "/value/init", &ret.init, status);
	if (gru_status_error(status)) {
		return ret;
	}

	bmic_java_read_mem_value(
		usage, "committed", "/value/committed", &ret.committed, status);
	if (gru_status_error(status)) {
		return ret;
	}

	bmic_java_read_mem_value(usage, "max", "/value/max", &ret.max, status);
	if (gru_status_error(status)) {
		return ret;
	}

	bmic_java_read_mem_value(usage, "used", "/value/used", &ret.used, status);
	if (gru_status_error(status)) {
		return ret;
	}

	return ret;
}

bmic_java_mem_info_t bmic_java_mem_eden_info(
	bmic_handle_t *handle, gru_status_t *status) {
	const bmic_object_t *usage = bmic_jolokia_io_read_attribute(
		handle, "java.lang", "name=PS Eden Space,type=MemoryPool", "Usage", status);

	bmic_java_mem_info_t ret = bmic_java_read_mem_info_any(handle, usage, status);
	bmic_object_destroy((bmic_object_t **) &usage);
	return ret;
}

bmic_java_mem_info_t bmic_java_mem_survivor_info(
	bmic_handle_t *handle, gru_status_t *status) {
	const bmic_object_t *usage = bmic_jolokia_io_read_attribute(
		handle, "java.lang", "name=PS Survivor Space,type=MemoryPool", "Usage", status);

	bmic_java_mem_info_t ret = bmic_java_read_mem_info_any(handle, usage, status);
	bmic_object_destroy((bmic_object_t **) &usage);
	return ret;
}

bmic_java_mem_info_t bmic_java_mem_tenured_info(
	bmic_handle_t *handle, gru_status_t *status) {
	const bmic_object_t *usage = bmic_jolokia_io_read_attribute(
		handle, "java.lang", "name=PS Old Gen,type=MemoryPool", "Usage", status);

	bmic_java_mem_info_t ret = bmic_java_read_mem_info_any(handle, usage, status);
	bmic_object_destroy((bmic_object_t **) &usage);
	return ret;
}

bmic_java_mem_info_t bmic_java_mem_code_cache_info(
	bmic_handle_t *handle, gru_status_t *status) {
	const bmic_object_t *usage = bmic_jolokia_io_read_attribute(
		handle, "java.lang", "name=CodeCache,type=MemoryPool", "Usage", status);

	bmic_java_mem_info_t ret = bmic_java_read_mem_info_any(handle, usage, status);
	bmic_object_destroy((bmic_object_t **) &usage);
	return ret;
}

bmic_java_mem_info_t bmic_java_mem_metaspace_info(
	bmic_handle_t *handle, gru_status_t *status) {
	const bmic_object_t *usage = bmic_jolokia_io_read_attribute(
		handle, "java.lang", "name=Metaspace,type=MemoryPool", "Usage", status);

	bmic_java_mem_info_t ret = bmic_java_read_mem_info_any(handle, usage, status);
	bmic_object_destroy((bmic_object_t **) &usage);
	return ret;
}

bmic_java_mem_info_t bmic_java_mem_permgen_info(
	bmic_handle_t *handle, gru_status_t *status) {
	const bmic_object_t *usage = bmic_jolokia_io_read_attribute(
		handle, "java.lang", "name=PermGen,type=MemoryPool", "Usage", status);

	bmic_java_mem_info_t ret = bmic_java_read_mem_info_any(handle, usage, status);
	bmic_object_destroy((bmic_object_t **) &usage);
	return ret;
}

static const char *bmic_java_get_string(
	const bmic_object_t *root, const char *name, const char *path, gru_status_t *status) {
	const bmic_object_t *obj = bmic_object_find_by_path(root, path);

	if (!obj) {
		gru_status_set(status, GRU_FAILURE, "Unable to find %s", name);
		return NULL;
	}
	if (obj->type != BMIC_STRING) {
		gru_status_set(status, GRU_FAILURE, "Invalid data type for %s", name);
		return NULL;
	}

	return strdup(obj->data.str);
}

static int64_t bmic_java_get_number(
	const bmic_object_t *root, const char *name, const char *path, gru_status_t *status) {
	const bmic_object_t *obj = bmic_object_find_by_path(root, path);

	if (!obj) {
		gru_status_set(status, GRU_FAILURE, "Unable to find %s", name);
		return 0;
	}
	if (obj->type != BMIC_INTEGER) {
		gru_status_set(status, GRU_FAILURE, "Invalid data type for %s", name);
		return 0;
	}

	if (obj->data.number == -1) {
		return 0;
	} else {
		logger_t logger = gru_logger_get();

		logger(DEBUG, "Read value %" PRId64 " for %s", obj->data.number, path);

		return obj->data.number;
	}
}

static double bmic_java_get_double(
	const bmic_object_t *root, const char *name, const char *path, gru_status_t *status) {
	const bmic_object_t *obj = bmic_object_find_by_path(root, path);

	if (!obj) {
		gru_status_set(status, GRU_FAILURE, "Unable to find %s", name);
		return 0;
	}
	if (obj->type != BMIC_DOUBLE) {
		gru_status_set(status, GRU_FAILURE, "Invalid data type for %s", name);
		return 0;
	}

	return obj->data.d;
}

bmic_java_info_t bmic_java_read_info(bmic_handle_t *handle, gru_status_t *status) {
	bmic_java_info_t ret = {0};
	const bmic_object_t *runtime =
		bmic_jolokia_io_read_attribute(handle, "java.lang", "type=Runtime", "", status);

	if (!runtime) {
		return ret;
	}

	bmic_jolokia_translate_status(runtime, status);
	if (gru_status_error(status)) {
		goto exit;
	}

	const bmic_object_t *jvmname = bmic_object_find_by_path(runtime, "/value/VmName");
	if (!jvmname) {
		gru_status_set(status, GRU_FAILURE, "Unable to find JVM name");
		goto exit;
	}
	if (jvmname->type != BMIC_STRING) {
		gru_status_set(status, GRU_FAILURE, "Invalid data type for JVM name");
		goto exit;
	}

	ret.name = bmic_java_get_string(runtime, "JVM name", "/value/VmName", status);
	if (!ret.name) {
		goto exit;
	}

	ret.version =
		bmic_java_get_string(runtime, "JVM version", "/value/SpecVersion", status);
	if (!ret.version) {
		goto exit;
	}

	ret.jvm_package_version = bmic_java_get_string(
		runtime, "JVM package version", "/value/SystemProperties/java.version", status);
	if (!ret.jvm_package_version) {
		goto exit;
	}

	if (strncmp(ret.version, "1.6", 3) == 0 || strncmp(ret.version, "1.7", 3) == 0) {
		ret.memory_model = BMIC_JAVA_LEGACY;
	} else {
		ret.memory_model = BMIC_JAVA_MODERN;
	}

exit:
	bmic_object_destroy((bmic_object_t **) &runtime);
	return ret;
}

bmic_java_os_info_t bmic_java_read_os_info(bmic_handle_t *handle, gru_status_t *status) {
	bmic_java_os_info_t ret = {0};
	const bmic_object_t *runtime = bmic_jolokia_io_read_attribute(
		handle, "java.lang", "type=OperatingSystem", "", status);

	if (!runtime) {
		return ret;
	}

	bmic_jolokia_translate_status(runtime, status);
	if (gru_status_error(status)) {
		goto exit;
	}

	ret.name = bmic_java_get_string(runtime, "OS name", "/value/Name", status);
	if (!ret.name) {
		goto exit;
	}

	ret.version = bmic_java_get_string(runtime, "OS version", "/value/Version", status);
	if (!ret.version) {
		goto exit;
	}

	ret.arch = bmic_java_get_string(runtime, "OS arch", "/value/Arch", status);
	if (!ret.arch) {
		goto exit;
	}

	ret.cpus = bmic_java_get_number(
		runtime, "processor count", "/value/AvailableProcessors", status);
	if (gru_status_error(status)) {
		goto exit;
	}

	ret.mem_total = bmic_java_get_number(
		runtime, "total memory", "/value/TotalPhysicalMemorySize", status);
	if (gru_status_error(status)) {
		goto exit;
	}

	ret.mem_free = bmic_java_get_number(
		runtime, "total free memory", "/value/FreePhysicalMemorySize", status);
	if (gru_status_error(status)) {
		goto exit;
	}

	ret.swap_total = bmic_java_get_number(
		runtime, "total swap memory", "/value/FreeSwapSpaceSize", status);
	if (gru_status_error(status)) {
		goto exit;
	}

	ret.swap_free = bmic_java_get_number(
		runtime, "total free swap memory", "/value/FreePhysicalMemorySize", status);
	if (gru_status_error(status)) {
		goto exit;
	}

	ret.swap_committed = bmic_java_get_number(runtime, "total committed swap memory",
		"/value/CommittedVirtualMemorySize", status);
	if (gru_status_error(status)) {
		goto exit;
	}

	ret.open_fd = bmic_java_get_number(
		runtime, "open file descriptors", "/value/OpenFileDescriptorCount", status);
	if (gru_status_error(status)) {
		goto exit;
	}

	ret.max_fd = bmic_java_get_number(
		runtime, "max file descriptors", "/value/MaxFileDescriptorCount", status);
	if (gru_status_error(status)) {
		goto exit;
	}

	ret.load_average =
		bmic_java_get_double(runtime, "load average", "/value/SystemLoadAverage", status);
	if (gru_status_error(status)) {
		goto exit;
	}

	ret.process_cpu_load = bmic_java_get_double(
		runtime, "process CPU load", "/value/ProcessCpuLoad", status);
	if (gru_status_error(status)) {
		goto exit;
	}

	ret.system_cpu_load =
		bmic_java_get_double(runtime, "system CPU load", "/value/SystemCpuLoad", status);
	if (gru_status_error(status)) {
		goto exit;
	}

	ret.process_cpu_time = bmic_java_get_number(
		runtime, "process CPU time", "/value/ProcessCpuTime", status);
	if (gru_status_error(status)) {
		goto exit;
	}

exit:
	bmic_object_destroy((bmic_object_t **) &runtime);
	return ret;
}