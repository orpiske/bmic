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
#include <string.h>
#include <getopt.h>

#include "bmic_operations_main.h"

typedef enum operations_t_ {
    OP_LIST,
    OP_CREATE,
    OP_DELETE,
} operations_t;

typedef struct options_t_
{
    char username[OPT_MAX_STR_SIZE];
    char password[OPT_MAX_STR_SIZE];
    char server[OPT_MAX_STR_SIZE];
    bool help;
    char name[OPT_MAX_STR_SIZE];
    operations_t operation;
    bool queue;
} options_t;


typedef struct cap_read_wrapper_t_ {
    bmic_handle_t *handle; 
    bmic_api_interface_t *api;
    const bmic_exchange_t *cap;
    gru_status_t *status;
} cap_read_wrapper_t;

static void print_op_arguments(const void *nodedata, void *p) {
    const bmic_op_arg_t *arg = (const bmic_op_arg_t *) nodedata;
    
    
    if (strncmp(arg->type, "void", 4) == 0) {
        printf("%-2s %-35s (void) %s\n", " ", arg->name, arg->description);
    }
    else { 
        printf("%-2s %10s %-35s %s \n", " ", bmic_type_map(arg->type), arg->name, 
               arg->description);
    }
}

static void print_op_signature(const void *nodedata, void *p) {
    const bmic_op_sig_t *sig = (const bmic_op_sig_t *) nodedata;
    const bmic_op_info_t *info = (bmic_op_info_t *) p;
    
    printf("\n%s%sDescription: %s%s\n", RESET, LIGHT_WHITE, RESET, sig->description);
    printf("%s %s()\n", bmic_type_map(sig->ret), info->name);
    
    printf("%s%s%-2s %10s %-35s %s%s\n", RESET, LIGHT_WHITE, " ", "Type", 
           "Parameter name", "Description", RESET);
    gru_list_for_each(sig->args, print_op_arguments, NULL);
}

static void print_op(const void *nodedata, void *payload)
{
    const bmic_op_info_t *info = (bmic_op_info_t *) nodedata;
    
    
    if (payload == NULL) { 
        printf("\n");
        gru_list_for_each(info->signature, print_op_signature, info);
    }
    else {
        const char *name = (const char *) payload;
        if (strncmp(info->name, name, strlen(info->name)) == 0) {
            printf("\n");
            gru_list_for_each(info->signature, print_op_signature, info);
        }
    }
}

static void show_help()
{
    printf("Usage: ");
    printf("\t-h\t--help show this help\n");
    printf("\t-u\t--username=<str> username to access the management console\n");
    printf("\t-p\t--password=<str> password to access the management console\n");
    printf("\t-s\t--server=<str> hostname or IP address of the server\n");
    printf("\t-l\t--list list available capabilities/attributes from the server\n");
    printf("\t-c\t--create create\n");
    printf("\t-c\t--delete delete\n");
    printf("\t-q\t--queue queue\n");
}


int operations_run(options_t *options)
{
    gru_status_t status = {0};

    bmic_context_t ctxt = {0};
    
    bool ret = bmic_context_init_simple(&ctxt, options->server, options->username, 
                             options->password, &status);
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
        case OP_LIST: {
            const bmic_list_t *list = api->operation_list(ctxt.handle, cap, &status);

            if (list) {           
                gru_list_for_each(list->items, print_op, NULL);
                bmic_list_destroy((bmic_list_t **)&list);
            }
            break;
        }
        case OP_CREATE: {
            api->create_queue(ctxt.handle, cap, options->name, &status);
            if (status.code != GRU_SUCCESS) {
                fprintf(stderr, "%s\n", status.message);
            }
            break;
        }
        case OP_DELETE: {
            api->delete_queue(ctxt.handle, cap, options->name, &status);
            if (status.code != GRU_SUCCESS) {
                fprintf(stderr, "%s\n", status.message);
            }
            break;
        }
        default: {
            if (options->help) { 
                const bmic_list_t *list = api->operation_list(ctxt.handle, cap, &status);

                if (list) {

                    gru_list_for_each(list->items, print_op, options->name);
                    bmic_list_destroy((bmic_list_t **)&list);
                }  
            }
            break;
        }
    }

    bmic_exchange_destroy((bmic_exchange_t **)&cap);
    bmic_context_cleanup(&ctxt);

    return EXIT_SUCCESS;
}

int operations_main(int argc, char **argv)
{
    
    int option_index = 0;
    options_t options = {0};

    if (argc < 2) {
        show_help();

        return EXIT_FAILURE;
    }

    while (1) {

        static struct option long_options[] = {
            { "help", optional_argument, 0, 'h'},
            { "username", required_argument, 0, 'u'},
            { "password", required_argument, 0, 'p'},
            { "server", required_argument, 0, 's'},
            { "list", no_argument, 0, 'l'},
            { "create", no_argument, 0, 'c'},
            { "delete", no_argument, 0, 'd'},
            { "queue", required_argument, 0, 'q'},
            { 0, 0, 0, 0}
        };

        int c = getopt_long(argc, argv, "h:u:p:s:l:cdq:", long_options, 
                            &option_index);
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
        case 'l':
            options.operation = OP_LIST;
            break;
        case 'c':
            options.operation = OP_CREATE;
            break;
        case 'd':
            options.operation = OP_DELETE;
            break;
        case 'q':
            strncpy(options.name, optarg, sizeof (options.name) - 1);
            break;
        case 'h':
            if (!optarg) {
                show_help();
                
                return EXIT_SUCCESS;
            }
            else {
                options.help = true;
                strncpy(options.name, optarg, sizeof (options.name) - 1);
            }
            
            break;
        default:
            printf("Invalid or missing option\n");
            show_help();
            return EXIT_FAILURE;
        }
    }

    if (options.operation != OP_LIST && strlen(options.name) == 0) {
        fprintf(stderr, "Either -l (--list) or -h <name> (--help=<name>) must be used\n");

        return EXIT_FAILURE;
    }

    return operations_run(&options);

}