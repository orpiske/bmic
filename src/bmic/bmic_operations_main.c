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
} operations_t;

typedef struct options_t_
{
    credential_options_t credentials;
    bmic_discovery_hint_t *hint;
    bool help;
    char name[OPT_MAX_STR_SIZE];
    operations_t operation;
    bool show_info;
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
    bmic_op_info_t *info = (bmic_op_info_t *) nodedata;
    
    
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


static void show_help(char **argv)
{
    print_program_usage(argv[0]);
    
    print_option_help("help", "h", "show this help");
    print_option_help("help=[operation]", "h [operation]", "show help for the given operation");
    print_option_help("username", "u", "username to access the management console");
    print_option_help("password", "p", "password to access the management console");
    print_option_help("server", "s", "server hostname or IP address");
    print_option_help("list", "l", "list available capabilities/attributes from the server");
    print_option_help("info", "I", "show server information during start-up");
}


int operations_run(options_t *options)
{
    gru_status_t status = {0};

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

        return EXIT_FAILURE;
    }    
    
    
    show_info(api, ctxt.handle, cap, options->show_info, &status);

    switch (options->operation) {
        case OP_LIST: {
            const bmic_list_t *list = api->operation_list(ctxt.handle, cap, &status);

            if (list) {           
                gru_list_for_each(list->items, print_op, NULL);
                bmic_list_destroy((bmic_list_t **)&list);
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
        show_help(argv);

        return EXIT_FAILURE;
    }
    
    options.show_info = false;
    gru_status_t status = {0};
    options.hint = bmic_discovery_hint_new(&status);
    
    if (!options.hint) {
        fprintf(stderr, "%s", status.message);
        
        return EXIT_FAILURE;
    }

    while (1) {

        static struct option long_options[] = {
            { "help", optional_argument, 0, 'h'},
            { "username", required_argument, 0, 'u'},
            { "password", required_argument, 0, 'p'},
            { "server", required_argument, 0, 's'},
            { "port", required_argument, 0, 'P'},
            { "url", required_argument, 0, 'U'},
            { "list", no_argument, 0, 'l'},
            { "info", no_argument, 0, 'I'},
            { 0, 0, 0, 0}
        };

        int c = getopt_long(argc, argv, "h:u:p:s:P:U:l:I", long_options, 
                            &option_index);
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
            bmic_discovery_hint_set_addressing_hostname(options.hint, optarg, &status);
            if (status.code != GRU_SUCCESS) {
                fprintf(stderr, "%s", status.message);
        
                return EXIT_FAILURE;
            }
            
            break;
        case 'P':
            bmic_discovery_hint_set_addressing_port(options.hint, atoi(optarg), &status);
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
        case 'I':
            options.show_info = true;
            break;
        case 'l':
            options.operation = OP_LIST;
            break;
        case 'h':
            if (!optarg) {
                show_help(argv);
                
                return EXIT_SUCCESS;
            }
            else {
                options.help = true;
                strlcpy(options.name, optarg, sizeof (options.name));
            }
            
            break;
        default:
            printf("Invalid or missing option\n");
            show_help(argv);
            return EXIT_FAILURE;
        }
    }

    if (options.operation != OP_LIST && strlen(options.name) == 0) {
        fprintf(stderr, "Either -l (--list) or -h <name> (--help=<name>) must be used\n");

        return EXIT_FAILURE;
    }

    return operations_run(&options);

}