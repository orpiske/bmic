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
#include "bmic_discovery_main.h"

static void show_help(char **argv) {
	gru_cli_program_usage("bmic", argv[0]);

	gru_cli_option_help("help", "h", "show this help");
	gru_cli_option_help("username [username]", "u [username]", 
		"username to access the management console");
	gru_cli_option_help("password [password]", "p [password]", 
		"password to access the management console");
	gru_cli_option_help("server [server]", "s [server]", "server hostname or IP address");
	gru_cli_option_help("url [url]", "U [url]", "management interface URL");
}

int discovery_run(options_t *options) {
	gru_status_t status = gru_status_new();

	bmic_context_t ctxt = {0};

	bool ret = bmic_context_init_hint(&ctxt,
		options->hint,
		options->credentials.username,
		options->credentials.password,
		&status);

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

	show_info(api, ctxt.handle, cap, true, &status);

	bmic_context_cleanup(&ctxt);
	return EXIT_SUCCESS;
}

int discovery_main(int argc, char **argv) {
	int option_index = 0;
	options_t options = options_init(DISCOVERY);

	if (argc < 2) {
		show_help(argv);

		return EXIT_FAILURE;
	}

	gru_status_t status = gru_status_new();

	while (1) {

		static struct option long_options[] = {{"help", no_argument, 0, 'h'},
			{"username", required_argument, 0, 'u'},
			{"password", required_argument, 0, 'p'},
			{"server", required_argument, 0, 's'},
			{"port", required_argument, 0, 'P'},
			{"url", required_argument, 0, 'U'},
			{0, 0, 0, 0}};

		int c = getopt_long(argc, argv, "hu:p:s:P:U:", long_options, &option_index);
		if (c == -1) {
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
					&options.hint, optarg, &status);
				if (gru_status_error(&status)) {
					fprintf(stderr, "%s", status.message);

					return EXIT_FAILURE;
				}

				break;
			case 'P':
				bmic_discovery_hint_set_addressing_port(
					&options.hint, (uint16_t) atoi(optarg), &status);
				if (gru_status_error(&status)) {
					fprintf(stderr, "%s", status.message);

					return EXIT_FAILURE;
				}

				break;
			case 'U':
				bmic_discovery_hint_set_url(&options.hint, optarg, &status);
				if (gru_status_error(&status)) {
					fprintf(stderr, "%s", status.message);

					return EXIT_FAILURE;
				}

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

	return discovery_run(&options);
}
