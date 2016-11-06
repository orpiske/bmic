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
#include "bmic_capabilities_main.h"

typedef struct options_t_
{
    char username[OPT_MAX_STR_SIZE];
    char password[OPT_MAX_STR_SIZE];
    char server[OPT_MAX_STR_SIZE];
    bool list;
    char read[OPT_MAX_STR_SIZE];
} options_t;

static void print_cap(const void *nodedata, void *payload)
{
    const char *cap = (const char *) nodedata;
    printf("-\t %s\n", cap);
}

static void show_help()
{
    printf("Usage: ");
    printf("\t-h\t--help show this help\n");
    printf("\t-u\t--username=<str> username to access the management console\n");
    printf("\t-p\t--password=<str> password to access the management console\n");
    printf("\t-s\t--server=<str> hostname or IP address of the server\n");
    printf("\t-l\t--list list available capabilities/attributes from the server\n");
    printf("\t-r\t--read=<str> read a capability/attribute from the server\n");
}

static void print_returned_object(const options_t *options, const bmic_object_t *obj)
{
    switch (obj->type) {
    case STRING:
    {
        printf("The value for capability %s is: %s\n", options->read,
               obj->data.str);
        break;
    }
    case INTEGER:
    {
        printf("The value for capability %s is: %i\n", options->read,
               obj->data.number);
        break;
    }
    case BOOLEAN:
    {
        printf("The value for capability %s is: %s\n", options->read,
               (obj->data.value ? "true" : "false"));
        break;
    }
    case DOUBLE:
    {
        printf("The value for capability %s is: %.4f\n", options->read,
               obj->data.d);
        break;
    }
    case NULL_TYPE:
    {
        printf("The value for capability %s is: (null)\n", options->read);
        break;
    }
    case LIST:
    case OBJECT:
    default:
    {
        printf("Unexpected complex object type was returned\n");
        break;
    }
    }
}

int capabilities_run(options_t *options)
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


    const bmic_product_cap_t *cap = api->capabilities(handle, &status);
    if (!cap) {
        fprintf(stderr, "Unable to load capabilities: %s\n", status.message);

        return EXIT_FAILURE;
    }


    if (options->list) {
        const gru_list_t *list = api->cap_all(handle, cap, &status);

        if (list) {
            printf("The following capabilities are available for the product:\n");
            gru_list_for_each(list, print_cap, NULL);
        }
    }
    else {
        const bmic_object_t *obj = api->cap_read(handle, cap, options->read,
                                                 &status);

        if (obj) {
            print_returned_object(options, obj);
        }
        else {
            printf("Unable to read the capability %s (wrong cap, maybe?)\n",
                   options->read);
        }
    }


    api->api_cleanup(&handle);
    bmic_product_unregister();
    bmic_product_registry_destroy();
    bmic_discovery_hint_destroy(&hint);
    bmic_credentials_detroy(&credentials);

    return EXIT_SUCCESS;
}

int capabilities_main(int argc, char **argv)
{
    int c = 0;
    int option_index = 0;
    options_t options = {0};

    options.list = false;

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
            { "list", false, 0, 'l'},
            { "read", true, 0, 'r'},
            { 0, 0, 0, 0}
        };

        c = getopt_long(argc, argv, "hu:p:l:s:lr:", long_options, &option_index);
        if (c == -1) {
            if (optind == 1) {
                // Will use defaults from the configuration file
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
        case 'l':
            options.list = true;
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

    if (options.list == false && strlen(options.read) == 0) {
        fprintf(stderr, "Either -l (--list) or -r (--read) must be used\n");

        return EXIT_FAILURE;
    }

    return capabilities_run(&options);

}