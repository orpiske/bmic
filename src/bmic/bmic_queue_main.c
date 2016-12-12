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

typedef enum operations_t_
{
    OP_LIST,
    OP_READ,
    OP_CREATE,
    OP_DELETE,
    OP_STATS,
} operations_t;

typedef struct options_t_
{
    credential_options_t credentials;
    operations_t operation;
    char server[OPT_MAX_STR_SIZE];
    char queue[OPT_MAX_STR_SIZE];
    char attribute[OPT_MAX_STR_SIZE];
    bool show_info;
} options_t;

static void show_help(char **argv)
{
    print_program_usage(argv[0]);

    print_option_help("help", "h", "show this help");
    print_option_help("help=[operation]", "h [operation]", "show help for the given operation");
    print_option_help("username", "u", "username to access the management console");
    print_option_help("password", "p", "password to access the management console");
    print_option_help("server", "s", "server hostname or IP address");
    print_option_help("name", "n", "name of the queue to manage");
    print_option_help("attribute", "a", "queue attribute to read");
    print_option_help("list", "l", "list queues from the server");
    print_option_help("stats", "S", "show queue statistics");
    print_option_help("info", "I", "show server information during start-up");
    
}

static void print_queue(const void *nodedata, void *payload)
{
    const char *name = (const char*) nodedata;

    printf("Queue name: %s\n", name);
}

static void print_queue_stat(const char *name, bmic_queue_stat_t stat)
{
    printf("Size: %"PRId64"\n", stat.queue_size);
    printf("Consumers: %"PRId64"\n", stat.consumer_count);
    printf("Ack Count: %"PRId64"\n", stat.msg_ack_count);
    printf("Exp Count: %"PRId64"\n", stat.msg_exp_count);
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
    const bmic_exchange_t *cap = api->capabilities_load(ctxt.handle, &status);
    if (!cap) {
        fprintf(stderr, "Unable to load capabilities: %s\n", status.message);

        return EXIT_FAILURE;
    }
    
    show_info(api, ctxt.handle, cap, options->show_info, &status);

    switch (options->operation) {
    case OP_READ:
    {
        const bmic_exchange_t *obj = api->queue_attribute_read(ctxt.handle, cap,
                                                               options->attribute, &status,
                                                               options->queue);

        if (obj) {
            print_returned_object(options->attribute, obj->data_ptr);
        }

        break;
    }
    case OP_LIST:
    {
        const bmic_list_t *list = api->queue_list(ctxt.handle, cap, &status);

        if (list) {
            gru_list_for_each(list->items, print_queue, NULL);
            
            bmic_list_destroy((bmic_list_t **) &list);
        }
        
        break;
    }
    case OP_CREATE:
    {
        api->queue_create(ctxt.handle, cap, options->queue, &status);
        if (status.code != GRU_SUCCESS) {
            fprintf(stderr, "%s\n", status.message);
        }
        break;
    }
    case OP_DELETE:
    {
        api->queue_delete(ctxt.handle, cap, options->queue, &status);
        if (status.code != GRU_SUCCESS) {
            fprintf(stderr, "%s\n", status.message);
        }
        break;
    }
    case OP_STATS:
    {
        bmic_queue_stat_t stats = api->queue_stats(ctxt.handle, cap, options->queue, &status);
        if (status.code != GRU_SUCCESS) {
            fprintf(stderr, "%s\n", status.message);
        }
        else {
            print_queue_stat(options->queue, stats);
        }
        break;
    }
    default:
    {
        fprintf(stderr, "Unable to read property %s for queue %s: %s\n", options->attribute,
                options->queue, status.message);
        break;
    }

    }

    bmic_exchange_destroy((bmic_exchange_t **) & cap);
    bmic_context_cleanup(&ctxt);

    return EXIT_FAILURE;
}

int queue_main(int argc, char **argv)
{
    int option_index = 0;
    options_t options = {0};
    
    options.show_info = false;

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
            { "name", required_argument, 0, 'n'},
            { "attribute", required_argument, 0, 'a'},
            { "list", no_argument, 0, 'l'},
            { "read", no_argument, 0, 'r'},
            { "create", no_argument, 0, 'c'},
            { "delete", no_argument, 0, 'd'},
            { "info", no_argument, 0, 'I'},
            { "stats", no_argument, 0, 'S'},
            { 0, 0, 0, 0}
        };

        int c = getopt_long(argc, argv, "hu:p:s:n:a:lrcdIS", long_options, &option_index);
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
        case 'c':
            options.operation = OP_CREATE;
            break;
        case 'd':
            options.operation = OP_DELETE;
            break;
        case 'S':
            options.operation = OP_STATS;
            break;
        case 'I':
            options.show_info = true;
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

    if (options.operation != OP_LIST && strlen(options.queue) == 0) {
        fprintf(stderr, "Option -n (--name) for all read/write operations\n");

        return EXIT_FAILURE;
    }

    return queue_run(&options);
}