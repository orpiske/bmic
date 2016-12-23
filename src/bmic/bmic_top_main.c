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
#include "bmic_top_main.h"

#define as_mb(x) (x / (1024 * 1024))

static void show_help(char **argv) {
	print_program_usage(argv[0]);

	print_option_help("help", "h", "show this help");

	print_option_help("username", "u", "username to access the management console");
	print_option_help("password", "p", "password to access the management console");
	print_option_help("server", "s", "server hostname or IP address");
	print_option_help("interval", "i", "interval between each update");
	print_option_help("attribute", "a", "queue attribute to read");
	print_option_help("repeat", "r",
		"how many times to read the data (-1, the default, means repeat forever)");
}

static void print_queue_stat(const char *name, bmic_queue_stat_t stat) {
	printf("%-40s %-9" PRId64 " %-9" PRId64 " %-9" PRId64 " %-9" PRId64 "\n", name,
		stat.queue_size, stat.consumer_count, stat.msg_ack_count, stat.msg_exp_count);
}

static void print_mem(const char *name, bmic_java_mem_info_t *mem) {
	printf("%-20s %-14" PRId64 " %-14" PRId64 " %-14" PRId64 " %-14" PRId64 "\n", name,
		as_mb(mem->init), as_mb(mem->committed), as_mb(mem->max), as_mb(mem->used));
}

int top_run(options_t *options) {
	gru_status_t status = gru_status_new();

	bmic_context_t ctxt = {0};

	bool ret = bmic_context_init_hint(&ctxt, options->hint, options->credentials.username,
		options->credentials.password, &status);

	if (!ret) {
		fprintf(stderr, "%s\n", status.message);
		return EXIT_FAILURE;
	}
	bmic_api_interface_t *api = ctxt.api;

	const bmic_exchange_t *cap = api->capabilities_load(ctxt.handle, &status);
	if (!cap) {
		fprintf(stderr, "Unable to load capabilities: %s\n", status.message);

		bmic_context_cleanup(&ctxt);
		return EXIT_FAILURE;
	}

	printf("Reading broker information ...\n");
	bmic_product_info_t *info = api->product_info(ctxt.handle, cap, &status);

	if (!info || status.code != GRU_SUCCESS) {
		fprintf(stderr, "Unable to determine product version: %s\n", status.message);

		bmic_context_cleanup(&ctxt);
		return EXIT_FAILURE;
	}

	bmic_java_info_t jinfo = api->java.java_info(ctxt.handle, &status);

	uint32_t iteration = 0;
	while (iteration != options->program.top.repeat) {
		size_t size = 0;
		bmic_queue_stat_t *stat = NULL;

		const bmic_list_t *list = api->queue_list(ctxt.handle, cap, &status);
		if (list && list->items) {
			size = gru_list_count(list->items);
		}

		if (size > 0) {
			stat = calloc(size, sizeof(bmic_queue_stat_t));
			if (!stat) {
				fprintf(stderr, "Unable to allocate memory for queue stats\n");

				break;
			}
		}

		bmic_java_os_info_t osinfo = api->java.os_info(ctxt.handle, &status);
		bmic_java_mem_info_t eden = api->java.eden_info(ctxt.handle, &status);
		bmic_java_mem_info_t survivor = api->java.survivor_info(ctxt.handle, &status);
		bmic_java_mem_info_t tenured = api->java.tenured_info(ctxt.handle, &status);
		bmic_java_mem_info_t metaspace;
		bmic_java_mem_info_t permgen;

		if (jinfo.memory_model == BMIC_JAVA_MODERN) {
			metaspace = api->java.metaspace_info(ctxt.handle, &status);
		} else {
			permgen = api->java.permgen_info(ctxt.handle, &status);
		}

		for (uint32_t i = 0; i < size; i++) {
			const gru_node_t *node = gru_list_get(list->items, i);

			if (node != NULL && node->data != NULL) {
				stat[i] = api->queue_stats(
					ctxt.handle, cap, (const char *) node->data, &status);

				if (status.code != GRU_SUCCESS) {
					break;
				}
			}
		}

		if (status.code != GRU_SUCCESS) {
			fprintf(stderr, "\n%s\n", status.message);

			if (list) {
				bmic_list_destroy((bmic_list_t **) &list);
			}

			if (size > 0) {
				free(stat);
			}

			break;
		}

		printf(CLEAR_SCREEN);
		printf("%s %s (%s) %s %s \n", jinfo.name, jinfo.version,
			jinfo.jvm_package_version, osinfo.name, osinfo.version);

		printf("%s%sCPUs:%s %" PRId64 "\n", RESET, LIGHT_WHITE, RESET, osinfo.cpus);
		printf("%s%sLoad average:%s %-10.1f\n", RESET, LIGHT_WHITE, RESET,
			osinfo.load_average);
		printf("%s%sFile descriptors:%s %10" PRId64 " max total %10" PRId64
			   " open %10" PRId64 " free\n",
			RESET, LIGHT_WHITE, RESET, osinfo.max_fd, osinfo.open_fd,
			(osinfo.max_fd - osinfo.open_fd));
		printf("%s%sPhysical memory:%s %10" PRId64 " total %10" PRId64 " free\n", RESET,
			LIGHT_WHITE, RESET, as_mb(osinfo.mem_total), as_mb(osinfo.mem_free));
		printf("%s%sSwap memory:%s %10" PRId64 " total %10" PRId64 " free %10" PRId64
			   " used\n\n",
			RESET, LIGHT_WHITE, RESET, as_mb(osinfo.swap_total), as_mb(osinfo.swap_free),
			as_mb(osinfo.swap_committed));

		printf("%s%s%s%-20s %-14s %-14s %-14s %-14s%s\n", RESET, BG_WHITE, LIGHT_BLACK,
			"Area", "Initial", "Committed", "Max", "Used", RESET);

		print_mem("Eden", &eden);
		print_mem("Survivor", &survivor);
		print_mem("Tenured", &tenured);

		if (jinfo.memory_model == BMIC_JAVA_MODERN) {
			print_mem("Metaspace", &metaspace);
		} else {
			print_mem("PermGen", &permgen);
		}

		if (size > 0) {
			printf("\n\n");
			printf("%s%s%s%-40s %-9s %-9s %-9s %-9s%s\n", RESET, BG_WHITE, LIGHT_BLACK,
				"Name", "Size", "Consumers", "Ack Count", "Exp Count", RESET);

			for (uint32_t i = 0; i < size; i++) {
				const gru_node_t *node = gru_list_get(list->items, i);

				if (node != NULL && node->data != NULL) {
					print_queue_stat((const char *) node->data, (stat[i]));
				}
			}

			free(stat);
		}

		printf("\n\n");
		printf("%s%s%s%-32s %-9s %-17s %-19s%s", RESET, BG_WHITE, LIGHT_BLACK,
			bmic_discovery_hint_host(options->hint), info->name, info->version, "Idle",
			RESET);

		fflush(NULL);
		bmic_java_os_info_cleanup(osinfo);

		if (list) {
			bmic_list_destroy((bmic_list_t **) &list);
		}

		if (options->program.top.repeat != -1) {
			iteration++;
			if (options->program.top.repeat == iteration) {
				break;
			}
		}

		sleep(options->program.top.interval);
		printf("%c[2K\r", 27);
		printf("%s%s%s%-32s %-9s %-17s %-19s%s", RESET, BG_WHITE, LIGHT_BLACK,
			bmic_discovery_hint_host(options->hint), info->name, info->version,
			"Reading...", RESET);
		fflush(NULL);
	}

	gru_dealloc((void **) &info);
	bmic_java_info_cleanup(jinfo);

	bmic_context_cleanup(&ctxt);
	return EXIT_SUCCESS;
}

int top_main(int argc, char **argv) {
	int option_index = 0;
	options_t options = options_init(TOP);

	if (argc < 2) {
		show_help(argv);

		return EXIT_FAILURE;
	}

	gru_status_t status = gru_status_new();
	options.hint = bmic_discovery_hint_new(&status);

	if (!options.hint) {
		fprintf(stderr, "%s", status.message);

		return EXIT_FAILURE;
	}

	while (1) {

		static struct option long_options[] = {{"help", no_argument, 0, 'h'},
			{"username", required_argument, 0, 'u'},
			{"password", required_argument, 0, 'p'},
			{"server", required_argument, 0, 's'}, {"port", required_argument, 0, 'P'},
			{"url", required_argument, 0, 'U'}, {"interval", required_argument, 0, 'i'},
			{"repeat", required_argument, 0, 'r'}, {0, 0, 0, 0}};

		int c = getopt_long(argc, argv, "hu:p:s:P:U:i:r:", long_options, &option_index);
		if (c == -1) {
			if (optind == 1) {
				break;
			}
			break;
		}

		switch (c) {
			case 'u':
				if (asprintf(&options.credentials.username, "%s", optarg) == -1) {
					fprintf(stderr, "Not enough memory to save username\n");
					return EXIT_FAILURE;
				}
				break;
			case 'p':
				if (asprintf(&options.credentials.password, "%s", optarg) == -1) {
					fprintf(stderr, "Not enough memory to save password\n");

					return EXIT_FAILURE;
				}

				break;
			case 's':
				bmic_discovery_hint_set_addressing_hostname(
					options.hint, optarg, &status);
				if (status.code != GRU_SUCCESS) {
					fprintf(stderr, "%s", status.message);

					return EXIT_FAILURE;
				}

				break;
			case 'P':
				bmic_discovery_hint_set_addressing_port(
					options.hint, atoi(optarg), &status);
				if (status.code != GRU_SUCCESS) {
					fprintf(stderr, "%s", status.message);

					return EXIT_FAILURE;
				}

				break;
			case 'U':
				bmic_discovery_hint_set_url(options.hint, optarg, &status);
				if (status.code != GRU_SUCCESS) {
					fprintf(stderr, "%s", status.message);

					return EXIT_FAILURE;
				}

				break;
			case 'i':
				options.program.top.interval = atoi(optarg);
				break;
			case 'r':
				options.program.top.repeat = atoi(optarg);
				break;
			case 'h':
				show_help(argv);
				return EXIT_SUCCESS;
			default:
				printf("Invalid or missing option\n");
				show_help(argv);
				return EXIT_FAILURE;
		}
	}

	return top_run(&options);
}
