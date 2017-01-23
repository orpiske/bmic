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
#include <stdio.h>
#include <stdlib.h>

#include <common/gru_colors.h>
#include <common/gru_status.h>
#include <cli/gru_cli_opt.h>

#include "bmic_capabilities_main.h"
#include "bmic_discovery_main.h"
#include "bmic_operations_main.h"
#include "bmic_queue_main.h"
#include "bmic_topic_main.h"
#include "bmic_top_main.h"


void show_help() {
	gru_cli_general_usage("bmic");

	gru_cli_program_description(
		"discovery", "Run a discovery on the broker to find its type and version");
	gru_cli_program_description("capabilities", "Read/write/list broker capabilities and attributes");
	gru_cli_program_description("queue", "Create/delete/list queues or read queue attributes");
	gru_cli_program_description("topic", "(EXPERIMENTAL) Create/delete/list topics or read topics attributes");
	gru_cli_program_description("operations", "Execute/list operations on the broker");
	gru_cli_program_description("top", "A top-like performance monitor for the broker");
}
int main(int argc, char **argv) {

	if (argc < 2) {
		show_help();

		return EXIT_FAILURE;
	} else {
		if (strcmp(argv[1], "capabilities") == 0) {
			return capabilities_main((argc - 1), &argv[1]);
		}

		if (strcmp(argv[1], "discovery") == 0) {
			return discovery_main((argc - 1), &argv[1]);
		}

		if (strcmp(argv[1], "queue") == 0) {
			return queue_main((argc - 1), &argv[1]);
		}

		if (strcmp(argv[1], "topic") == 0) {
			return topic_main((argc - 1), &argv[1]);
		}

		if (strcmp(argv[1], "operations") == 0) {
			return operations_main((argc - 1), &argv[1]);
		}

		if (strcmp(argv[1], "top") == 0) {
			return top_main((argc - 1), &argv[1]);
		}
	}

	return EXIT_SUCCESS;
}
