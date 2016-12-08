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

typedef struct options_t_
{
    credential_options_t credentials;
    char server[OPT_MAX_STR_SIZE];
    bool list;
} options_t;

static void show_help(char **argv)
{
    print_program_usage(argv[0]);
    
    print_option_help("help", "h", "show this help");
    print_option_help("username", "u", "username to access the management console");
    print_option_help("password", "p", "password to access the management console");
    print_option_help("server", "s", "server hostname or IP address");
}


int discovery_run(options_t *options)
{
   gru_status_t status = {0};

    bmic_context_t ctxt = {0};
    
    bool ret = bmic_context_init_simple(&ctxt, options->server, options->credentials.username, 
                             options->credentials.password, &status);
    
    if (!ret) {
        fprintf(stderr, "%s\n", status.message);
        return EXIT_FAILURE;
    }
    bmic_api_interface_t *api = ctxt.api;
    
    bmic_product_info_t *info = ctxt.api->product_info(ctxt.handle, &status);
    print_product_info(api, info);
    
    if (!info || status.code != GRU_SUCCESS) {
        fprintf(stderr, "Unable to determine product version: %s\n",
                status.message);
    }
    
    if (info) {
        gru_dealloc((void **) &info);
    }

    bmic_context_cleanup(&ctxt);

    return EXIT_SUCCESS;
}

int discovery_main(int argc, char **argv) {
    int option_index = 0;
    options_t options = {0};

    options.list = false;

    if (argc < 2) {
        show_help(argv);

        return EXIT_FAILURE;
    }

    while (1) {

        static struct option long_options[] = {
            { "help", no_argument, 0, 'h'},
            { "username", required_argument, 0, 'u'},
            { "password", required_argument, 0, 'p'},
            { "server", required_argument, 0, 's'},
            { 0, 0, 0, 0}
        };

        int c = getopt_long(argc, argv, "hu:p:s:", long_options, &option_index);
        if (c == -1) {
            if (optind == 1) {
                break;
            }
            break;
        }

        switch (c) {
        case 'u':
            strlcpy(options.credentials.username, optarg, sizeof (options.credentials.username));
            break;
        case 'p':
            strlcpy(options.credentials.password, optarg, sizeof (options.credentials.password));
            break;
        case 's':
            strlcpy(options.server, optarg, sizeof (options.server));
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