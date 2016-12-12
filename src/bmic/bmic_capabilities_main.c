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
    credential_options_t credentials;
    char server[OPT_MAX_STR_SIZE];
    bool list;
    bool readall;
    char read[OPT_MAX_STR_SIZE];
    bool show_info;
} options_t;

typedef struct cap_read_wrapper_t_ {
    bmic_handle_t *handle; 
    bmic_api_interface_t *api;
    const bmic_exchange_t *cap;
    gru_status_t *status;
} cap_read_wrapper_t;

static void print_cap_info(const bmic_cap_info_t *info)
{
    const char *typename_short = bmic_type_map(info->typename);
    
    printf("- %-35s %-5s %-15s %s\n", info->name, 
           (info->write ? "rw" : "ro"), typename_short,
           info->description);
}

static void print_cap(const void *nodedata, void *payload)
{
    const bmic_cap_info_t *info = (bmic_cap_info_t *) nodedata;
    print_cap_info(info);
}

static void show_help(char **argv)
{
    print_program_usage(argv[0]);
    
    print_option_help("help", "h", "show this help");
    print_option_help("username", "u", "username to access the management console");
    print_option_help("password", "p", "password to access the management console");
    print_option_help("server", "s", "server hostname or IP address");
    print_option_help("list", "l", "list available capabilities/attributes from the server");
    print_option_help("read=<str>", "r <str>", "read the capability/attribute named <str> from the server");
    print_option_help("read-all", "R", "read all available capabilities/attributes from the server");
    print_option_help("info", "I", "show server information during start-up");
}

void capabilities_do_read(bmic_handle_t *handle, 
                       bmic_api_interface_t *api, const bmic_exchange_t *cap, 
                        const char *capname, gru_status_t *status)
{
    const bmic_exchange_t *obj = api->attribute_read(handle, cap, capname,
                                             status);

    if (obj) {
        print_returned_object(capname, obj->data_ptr);
        
        bmic_exchange_destroy((bmic_exchange_t **) &obj);
    }
    else {
        if (status->code != GRU_SUCCESS) { 
            printf("%35s %s%s%s%s\n", capname, RESET, RED, status->message, RESET);
            gru_status_reset(status);
        }
        else {
            printf("%35s %s%s%s%s\n", capname, RESET, RED, 
                    "(null)", RESET);
        }
    }
    
    
}

static void capabilities_read(const void *nodedata, void *payload)
{
    const bmic_cap_info_t *info = (bmic_cap_info_t *) nodedata;
    cap_read_wrapper_t *wrapper = (cap_read_wrapper_t *) payload;
    
    capabilities_do_read(wrapper->handle, wrapper->api, wrapper->cap, 
                      info->name, wrapper->status);
    
}

int capabilities_run(options_t *options)
{
    gru_status_t status = {0};

    bmic_context_t ctxt = {0};
    
    bool ret = bmic_context_init_simple(&ctxt, options->server, 
                                        options->credentials.username, 
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

    if (options->list) {
        const bmic_list_t *list = api->attribute_list(ctxt.handle, cap, &status);

        if (list) {
            gru_list_for_each(list->items, print_cap, NULL);
            bmic_list_destroy((bmic_list_t **)&list);
        }
        
    }
    else {
        if (options->readall) {
            const bmic_list_t *list = api->attribute_list(ctxt.handle, cap, &status);

            if (list) {
                cap_read_wrapper_t wrapper; 
                
                wrapper.api = api;
                wrapper.cap = cap;
                wrapper.handle = ctxt.handle;
                wrapper.status = &status;
                
                printf("\n%s%s%35s %-25s%s\n", RESET, LIGHT_WHITE, "Capability", "Value", RESET);
                gru_list_for_each(list->items, capabilities_read, &wrapper);
                
                bmic_list_destroy((bmic_list_t **)&list);
            }
            
        }
        else {
            const bmic_exchange_t *obj = api->attribute_read(ctxt.handle, cap, options->read,
                                                     &status);

            if (obj) {
                printf("\n%s%s%35s %-25s%s\n", RESET, LIGHT_WHITE, "Capability", "Value", RESET);
                print_returned_object(options->read, obj->data_ptr);
            }
            else {
                printf("%35s %s%s%s%s\n", options->read, RESET, RED, "Unable to read", RESET);
            }
            bmic_exchange_destroy((bmic_exchange_t **)&obj);
        }
    }

    bmic_exchange_destroy((bmic_exchange_t **)&cap);
    bmic_context_cleanup(&ctxt);

    return EXIT_SUCCESS;
}

int capabilities_main(int argc, char **argv)
{
    
    int option_index = 0;
    options_t options = {0};

    options.list = false;
    options.readall = false;
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
            { "list", no_argument, 0, 'l'},
            { "read", required_argument, 0, 'r'},
            { "read-all", no_argument, 0, 'R'},
            { "info", no_argument, 0, 'I'},
            { 0, 0, 0, 0}
        };

        int c = getopt_long(argc, argv, "hu:p:l:s:lr:RI", long_options, 
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
            strlcpy(options.server, optarg, sizeof (options.server));
            break;
        case 'l':
            options.list = true;
            break;
        case 'r':
            strlcpy(options.read, optarg, sizeof (options.read));
            break;
        case 'R':
            options.readall = true;
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

    if (options.list == false && strlen(options.read) == 0 && options.readall == false) {
        fprintf(stderr, "Either -l (--list) or -r (--read) must be used\n");

        return EXIT_FAILURE;
    }

    return capabilities_run(&options);

}