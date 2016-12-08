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

typedef enum operations_t_ {
    OP_LIST,
    OP_READ,
    // OP_CREATE,
    // OP_DELETE,
} operations_t;

typedef struct options_t_
{
    credential_options_t credentials;
    operations_t operation;
    char server[OPT_MAX_STR_SIZE];
    char queue[OPT_MAX_STR_SIZE];
    char attribute[OPT_MAX_STR_SIZE];
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

static void print_queue(const void *nodedata, void *payload)
{
    const char *name = (const char*) nodedata;
    
    printf("Queue name: %s\n", name);
}

int queue_run(options_t *options)
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
    
    bmic_product_info_t *info = api->product_info(ctxt.handle, &status);
    print_product_info(api, info);
    
    if (!info || status.code != GRU_SUCCESS) {
        fprintf(stderr, "Unable to determine product version: %s\n",
                status.message);
    }
    
    if (info) {
        gru_dealloc((void **) &info);
    }

    const bmic_exchange_t *cap = api->load_capabilities(ctxt.handle, &status);
    if (!cap) {
        fprintf(stderr, "Unable to load capabilities: %s\n", status.message);

        return EXIT_FAILURE;
    }
    
    switch (options->operation) {
    case OP_READ: {
        const bmic_exchange_t *obj = api->queue_attribute_read(ctxt.handle, cap, 
                                                           options->attribute, &status, 
                              options->queue);

        if (obj) { 
            print_returned_object(options->attribute, obj->data_ptr);
        }
        
        break;
    }
    case OP_LIST: {
        const bmic_list_t *list = api->list_queues(ctxt.handle, cap, &status);

         if (list) {           
            gru_list_for_each(list->items, print_queue, NULL);
        }
        break;
    }
    default: {
        fprintf(stderr, "Unable to read property %s for queue %s: %s\n", options->attribute, 
                options->queue, status.message);
        break;
    }
        
    }

    bmic_exchange_destroy((bmic_exchange_t **)&cap);
    bmic_context_cleanup(&ctxt);
        
    return EXIT_FAILURE;    
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
            { "help", no_argument, 0, 'h'},
            { "username", required_argument, 0, 'u'},
            { "password", required_argument, 0, 'p'},
            { "server", required_argument, 0, 's'},
            { "name", required_argument, 0, 'n'},
            { "attribute", required_argument, 0, 'a'},
            { "list", no_argument, 0, 'l'},
            { "read", no_argument, 0, 'r'},
            { 0, 0, 0, 0}
        };

        int c = getopt_long(argc, argv, "hu:p:s:n:a:lr", long_options, &option_index);
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
        case 'n':
            strlcpy(options.queue, optarg, sizeof (options.queue));
            break;
        case 'a':
            strlcpy(options.attribute, optarg, sizeof (options.attribute));
            break;
        case 'l':
            options.operation = OP_LIST;
            break;
        case 'r':
            options.operation = OP_READ;
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

    if (options.operation != OP_LIST && strlen(options.queue) == 0) {
        fprintf(stderr, "Option -n (--name) for all read/write operations\n");

        return EXIT_FAILURE;
    }

    return queue_run(&options);
}