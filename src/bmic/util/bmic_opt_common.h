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
#ifndef BMIC_OPT_COMMON_H
#define BMIC_OPT_COMMON_H

#define OPT_MAX_STR_SIZE 255

#include <stdio.h>

#include <common/gru_colors.h>

#include "management/common/bmic_discovery_hint.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum operations_t_ {
	OP_LIST,
	OP_READ,
	OP_CREATE,
	OP_DELETE,
	OP_STATS,
} operations_t;

typedef struct credential_options_t_ {
	char *username;
	char *password;
} credential_options_t;

struct cap_options_t {
	bool list;
	bool readall;
	char *read;
	bool show_info;
};

struct op_options_t {
	char *name;
	operations_t operation;
	bool show_info;
};

struct queue_options_t {
	operations_t operation;
	char *queue;
	char *attribute;
	bool show_info;
};

struct top_options_t {
	int32_t interval;
	int32_t repeat;
};

typedef enum program_type_t_ {
	CAPABILITIES,
	OPERATIONS,
	DISCOVERY,
	QUEUE,
	TOP,
} program_type_t;

typedef struct options_t_ {
	program_type_t type;
	credential_options_t credentials;
	bmic_discovery_hint_t *hint;
	bool help;

	union program_options_t {
		struct cap_options_t capabilities;
		struct op_options_t operations;
		struct queue_options_t queue;
		struct top_options_t top;
	} program;

} options_t;

options_t options_init(program_type_t type);
void options_cleanup();

void print_option_help(const char *long_opt, const char *short_opt, const char *desc);
void print_program_usage(char *program_name);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_OPT_COMMON_H */
