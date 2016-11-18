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

typedef struct options_t_
{
    char username[OPT_MAX_STR_SIZE];
    char password[OPT_MAX_STR_SIZE];
    char server[OPT_MAX_STR_SIZE];
    char queue[OPT_MAX_STR_SIZE];
    char read[OPT_MAX_STR_SIZE];
} options_t;

static void show_help()
{
    printf("Usage: ");
    printf("\t-h\t--help show this help\n");
    printf("\t-u\t--username=<str> username to access the management console\n");
    printf("\t-p\t--password=<str> password to access the management console\n");
    printf("\t-s\t--server=<str> hostname or IP address of the server\n");
    printf("\t-l\t--name=<str> name of the queue to manage\n");
    printf("\t-r\t--read=<str> read a capability/attribute from the queue\n");
}

int queue_run(options_t *options)
{
    gru_status_t status = {0};

    bmic_discovery_hint_t *hint = NULL;
    bmic_credentials_t *credentials = NULL;

    bmic_product_registry_init(&status);

    bmic_product_register(&status);


    credentials = bmic_credentials_init(options->username, options->password,
                                        &status);

    hint = bmic_discovery_hint_eval_addressing(options->server,
                                               BMIC_PORT_UNKNOWN,
                                               &status);

    bmic_handle_t *handle = NULL;
    bmic_api_interface_t *api = bmic_discovery_run(hint, credentials, &handle,
                                                   &status);

    if (api == NULL) {
        fprintf(stderr, "Unable to discover server: %s\n", status.message);

        return EXIT_FAILURE;
    }

    printf("Product name is %s\n", api->name);
    printf("API version is %s\n", api->version);

    bmic_product_info_t *info = api->product_info(handle, &status);
    if (!info || status.code != GRU_SUCCESS) {
        fprintf(stderr, "Unable to determine product version: %s\n",
                status.message);
    }
    else {
        printf("The product version is: %s\n", info->version);
        gru_dealloc((void **) &info);
    }


    const bmic_exchange_t *cap = api->load_capabilities(handle, &status);
    if (!cap) {
        fprintf(stderr, "Unable to load capabilities: %s\n", status.message);

        return EXIT_FAILURE;
    }
    
    
    const bmic_exchange_t *obj = api->queue_attribute_read(handle, cap, 
                                                           options->read, &status, 
                              options->queue);
    
    print_returned_object("test", obj->data_ptr);
    
    return EXIT_SUCCESS;
}

int queue_main(int argc, char **argv) {
    int option_index = 0;
    options_t options = {0};

    if (argc < 2) {
        show_help();

        return EXIT_FAILURE;
    }

    while (1) {

        static struct option long_options[] = {
            { "help", false, 0, 'h'},
            { "username", true, 0, 'u'},
            { "password", true, 0, 'p'},
            { "server", true, 0, 's'},
            { "name", true, 0, 'n'},
            { "read", true, 0, 'r'},
            { 0, 0, 0, 0}
        };

        int c = getopt_long(argc, argv, "hu:p:s:n:r:", long_options, &option_index);
        if (c == -1) {
            if (optind == 1) {
                break;
            }
            break;
        }

        switch (c) {
        case 'u':
            strncpy(options.username, optarg, sizeof (options.username) - 1);
            break;
        case 'p':
            strncpy(options.password, optarg, sizeof (options.password) - 1);
            break;
        case 's':
            strncpy(options.server, optarg, sizeof (options.server) - 1);
            break;
        case 'n':
            strncpy(options.queue, optarg, sizeof (options.queue) - 1);
            break;
        case 'r':
            strncpy(options.read, optarg, sizeof (options.read) - 1);
            break;
        case 'h':
            show_help();
            return EXIT_SUCCESS;
        default:
            printf("Invalid or missing option\n");
            show_help();
            return EXIT_FAILURE;
        }
    }

    if (strlen(options.read) == 0 || strlen(options.queue) == 0) {
        fprintf(stderr, "Both -n (--name) and -r (--read) must be used\n");

        return EXIT_FAILURE;
    }

    return queue_run(&options);
}