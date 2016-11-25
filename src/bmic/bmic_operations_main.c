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

typedef struct options_t_
{
    char username[OPT_MAX_STR_SIZE];
    char password[OPT_MAX_STR_SIZE];
    char server[OPT_MAX_STR_SIZE];
    bool list;
    char name[OPT_MAX_STR_SIZE];
} options_t;




typedef struct cap_read_wrapper_t_ {
    bmic_handle_t *handle; 
    bmic_api_interface_t *api;
    const bmic_exchange_t *cap;
    gru_status_t *status;
} cap_read_wrapper_t;

static void print_op_arguments(const void *nodedata, void *p) {
    bmic_op_arg_t *arg = (const bmic_op_arg_t *) nodedata;
    
    printf("\t%-30s %-35s %-15s\n", arg->name, arg->description, arg->type);
}

static void print_op_signature(const void *nodedata, void *p) {
    const bmic_op_sig_t *sig = (const bmic_op_sig_t *) nodedata;
    
    printf("   %-65s %-15s\n", sig->description, sig->ret);
    gru_list_for_each(sig->args, print_op_arguments, NULL);
}

static void print_op(const void *nodedata, void *payload)
{
    const bmic_op_info_t *info = (bmic_op_info_t *) nodedata;
    
    if (payload == NULL) { 
        printf("- %-35s\n", info->name);
        
        gru_list_for_each(info->signature, print_op_signature, NULL);
    }
    else {
        const char *name = (const char *) payload;
        if (strncmp(info->name, name, strlen(info->name)) == 0) {
            printf("- %-35s\n", info->name);
            
            gru_list_for_each(info->signature, print_op_signature, NULL);
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
}


int operations_run(options_t *options)
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

    bmic_product_info_t *info = api->product_info(handle, &status);
    print_product_info(api, info);
    
    if (!info || status.code != GRU_SUCCESS) {
        fprintf(stderr, "Unable to determine product version: %s\n",
                status.message);
    }
    
    if (info) {
        gru_dealloc((void **) &info);
    }
    
    const bmic_exchange_t *cap = api->load_capabilities(handle, &status);
    if (!cap) {
        fprintf(stderr, "Unable to load capabilities: %s\n", status.message);

        return EXIT_FAILURE;
    }    

    if (options->list) {
        const bmic_list_t *list = api->operation_list(handle, cap, &status);

        if (list) {
            printf("\n  %-20s\n", "NAME");
            
            gru_list_for_each(list->items, print_op, NULL);
            bmic_list_destroy((bmic_list_t **)&list);
        }  
    }
    else {
        const bmic_list_t *list = api->operation_list(handle, cap, &status);
        
        if (list) {
            printf("\n  %-20s %-15s %-30s\n", "NAME", "RETURN", "DESCRIPTION");
            
            gru_list_for_each(list->items, print_op, options->name);
            bmic_list_destroy((bmic_list_t **)&list);
        }  
    }

    bmic_exchange_destroy((bmic_exchange_t **)&cap);
    api->api_cleanup(&handle);
    bmic_product_unregister();
    bmic_product_registry_destroy();
    bmic_discovery_hint_destroy(&hint);
    bmic_credentials_detroy(&credentials);

    return EXIT_SUCCESS;
}

int operations_main(int argc, char **argv)
{
    
    int option_index = 0;
    options_t options = {0};

    options.list = false;

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
            { 0, 0, 0, 0}
        };

        int c = getopt_long(argc, argv, "h:u:p:l:s:lr:", long_options, 
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
            options.list = true;
            break;
        case 'h':
            
            if (!optarg) {
                show_help();
                
                return EXIT_SUCCESS;
            }
            else {
                strncpy(options.name, optarg, sizeof (options.name) - 1);
            }
            
            break;
        default:
            printf("Invalid or missing option\n");
            show_help();
            return EXIT_FAILURE;
        }
    }

    if (options.list == false && strlen(options.name) == 0) {
        fprintf(stderr, "Either -l (--list) or -h <name> (--help=<name>) must be used\n");

        return EXIT_FAILURE;
    }

    return operations_run(&options);

}