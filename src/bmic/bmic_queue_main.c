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
#include "bmic_queue_main.h"

static void show_help(char **argv) {
	print_program_usage(argv[0]);

	print_option_help("help", "h", "show this help");
	print_option_help(
		"help=[operation]", "h [operation]", "show help for the given operation");
	print_option_help("username", "u", "username to access the management console");
	print_option_help("password", "p", "password to access the management console");
	print_option_help("server", "s", "server hostname or IP address");
	print_option_help("url", "U", "management interface URL");
	print_option_help("name", "n", "name of the queue to manage");
	print_option_help("attribute", "a", "queue attribute to read");
	print_option_help("list", "l", "list queues from the server");
	print_option_help("stats", "S", "show queue statistics");
	print_option_help("info", "I", "show server information during start-up");
	print_option_help("create", "c", "create a queue on the server");
	print_option_help("delete", "d", "create a queue on the server");
	print_option_help("purge", "G", "remove all messages from the queue");
}

static void print_queue(const void *nodedata, void *payload) {
	const char *name = (const char *) nodedata;

	printf("Queue name: %s\n", name);
}

static void print_queue_stat(const char *name, bmic_queue_stat_t stat) {
	printf("Size: %" PRId64 "\n", stat.queue_size);
	printf("Consumers: %" PRId64 "\n", stat.consumer_count);
	printf("Ack Count: %" PRId64 "\n", stat.msg_ack_count);
	printf("Exp Count: %" PRId64 "\n", stat.msg_exp_count);
}

int queue_run(options_t *options) {
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

	show_info(api, ctxt.handle, cap, options->program.queue.show_info, &status);

	switch (options->program.queue.operation) {
		case OP_READ: {
			const bmic_exchange_t *obj = api->queue_attribute_read(ctxt.handle, cap,
				options->program.queue.attribute, &status, options->program.queue.queue);

			if (obj) {
				print_returned_object(options->program.queue.attribute, obj->data_ptr);
			}

			break;
		}
		case OP_LIST: {
			const bmic_list_t *list = api->queue_list(ctxt.handle, cap, &status);

			if (list) {
				gru_list_for_each(list->items, print_queue, NULL);

				bmic_list_destroy((bmic_list_t **) &list);
			}

			break;
		}
		case OP_CREATE: {
			api->queue_create(ctxt.handle, cap, options->program.queue.queue, &status);
			if (status.code != GRU_SUCCESS) {
				fprintf(stderr, "%s\n", status.message);
			}
			break;
		}
		case OP_DELETE: {
			api->queue_delete(ctxt.handle, cap, options->program.queue.queue, &status);
			if (status.code != GRU_SUCCESS) {
				fprintf(stderr, "%s\n", status.message);
			}
			break;
		}
		case OP_PURGE: {
			api->queue_purge(ctxt.handle, cap, options->program.queue.queue, &status);
			if (status.code != GRU_SUCCESS) {
				fprintf(stderr, "%s\n", status.message);
			}
			break;
		}
		case OP_STATS: {
			bmic_queue_stat_t stats =
				api->queue_stats(ctxt.handle, cap, options->program.queue.queue, &status);
			if (status.code != GRU_SUCCESS) {
				fprintf(stderr, "%s\n", status.message);
			} else {
				print_queue_stat(options->program.queue.queue, stats);
			}
			break;
		}
		default: {
			fprintf(stderr, "Unable to read property %s for queue %s: %s\n",
				options->program.queue.attribute, options->program.queue.queue,
				status.message);
			break;
		}
	}

	bmic_context_cleanup(&ctxt);
	return EXIT_FAILURE;
}

int queue_main(int argc, char **argv) {
	int option_index = 0;
	options_t options = options_init(QUEUE);

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
			{"url", required_argument, 0, 'U'}, {"name", required_argument, 0, 'n'},
			{"attribute", required_argument, 0, 'a'}, {"list", no_argument, 0, 'l'},
			{"read", no_argument, 0, 'r'},
			{"create", no_argument, 0, 'c'},
			{"delete", no_argument, 0, 'd'},
			{"purge", no_argument, 0, 'G'},
			{"info", no_argument, 0, 'I'},
			{"stats", no_argument, 0, 'S'},
			{0, 0, 0, 0}};

		int c =
			getopt_long(argc, argv, "hu:p:s:P:U:n:a:lrcdGIS", long_options, &option_index);
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
					options.hint, (uint16_t) atoi(optarg), &status);
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
			case 'n':
				if (asprintf(&options.program.queue.queue, "%s", optarg) == -1) {
					fprintf(stderr, "Not enough memory to save queue name\n");

					return EXIT_FAILURE;
				}
				break;
			case 'a':
				if (asprintf(&options.program.queue.attribute, "%s", optarg) == -1) {
					fprintf(stderr, "Not enough memory to save attribute name\n");

					return EXIT_FAILURE;
				}
				break;
			case 'l':
				options.program.queue.operation = OP_LIST;
				break;
			case 'r':
				options.program.queue.operation = OP_READ;
				break;
			case 'c':
				options.program.queue.operation = OP_CREATE;
				break;
			case 'd':
				options.program.queue.operation = OP_DELETE;
				break;
			case 'G':
				options.program.queue.operation = OP_PURGE;
				break;
			case 'S':
				options.program.queue.operation = OP_STATS;
				break;
			case 'I':
				options.program.queue.show_info = true;
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

	if (options.program.queue.operation != OP_LIST &&
		options.program.queue.queue == NULL) {
		fprintf(stderr, "Option -n (--name) for all read/write operations\n");

		return EXIT_FAILURE;
	}

	return queue_run(&options);
}
