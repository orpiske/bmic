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
    bool readall;
    char read[OPT_MAX_STR_SIZE];
} options_t;

typedef struct cap_read_wrapper_t_ {
    bmic_handle_t *handle; 
    bmic_api_interface_t *api;
    const bmic_exchange_t *cap;
    gru_status_t *status;
} cap_read_wrapper_t;

static void print_cap_info(const bmic_cap_info_t *info)
{
    const char *typename_short = NULL;
    
    if (strstr(info->typename, "java.lang.String")) {
        typename_short = "string";
    }
    else {
        if (strstr(info->typename, "Object")) {
           typename_short = "object";
        }
        else { 
            typename_short = info->typename;
        }
    
    }
    
    printf("- %-35s %-5s %-15s %s\n", info->name, 
           (info->write ? "rw" : "ro"), typename_short,
           info->description);
}

static void print_cap(const void *nodedata, void *payload)
{
    const bmic_cap_info_t *info = (bmic_cap_info_t *) nodedata;
    print_cap_info(info);
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


void capabilities_do_read(bmic_handle_t *handle, 
                       bmic_api_interface_t *api, const bmic_exchange_t *cap, 
                        const char *capname, gru_status_t *status)
{
    const bmic_exchange_t *obj = api->attribute_read(handle, cap, capname,
                                             status);

    if (obj) {
        print_cap_info(obj->payload.capinfo);
        print_returned_object(capname, obj->data_ptr);
        
        bmic_exchange_destroy((bmic_exchange_t **) &obj);
    }
    else {
        printf("Unable to read the capability %s (wrong cap, maybe?): %s\n",
               capname, status->message);
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
        const bmic_list_t *list = api->attribute_list(handle, cap, &status);

        if (list) {
            printf("\n  %-35s %-5s %-15s %s\n", "NAME", "MODE", "TYPE", "DESCRIPTION");
            
            gru_list_for_each(list, print_cap, NULL);
            bmic_caplist_destroy((bmic_list_t **)&list);
        }
        
    }
    else {
        if (options->readall) {
            const bmic_list_t *list = api->attribute_list(handle, cap, &status);

            if (list) {
                cap_read_wrapper_t wrapper; 
                
                wrapper.api = api;
                wrapper.cap = cap;
                wrapper.handle = handle;
                wrapper.status = &status;
                
                printf("\n  %-35s %-5s %-15s %s\n", "NAME", "MODE", "TYPE", "DESCRIPTION");
                gru_list_for_each(list, capabilities_read, &wrapper);
                
                bmic_caplist_destroy((bmic_list_t **)&list);
            }
            
        }
        else {
            const bmic_exchange_t *obj = api->attribute_read(handle, cap, options->read,
                                                     &status);

            if (obj) {
                printf("\n  %-35s %-5s %-15s %s\n", "NAME", "MODE", "TYPE", "DESCRIPTION");
                print_cap_info(obj->payload.capinfo);
                print_returned_object(options->read, obj->data_ptr);
            }
            else {
                printf("Unable to read the capability %s (wrong cap, maybe?)\n",
                       options->read);
            }
            bmic_exchange_destroy((bmic_exchange_t **)&obj);
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

int capabilities_main(int argc, char **argv)
{
    
    int option_index = 0;
    options_t options = {0};

    options.list = false;
    options.readall = false;

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
            { "read-all", false, 0, 'R'},
            { 0, 0, 0, 0}
        };

        int c = getopt_long(argc, argv, "hu:p:l:s:lr:", long_options, 
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
        case 'r':
            strncpy(options.read, optarg, sizeof (options.read) - 1);
            break;
        case 'R':
            options.readall = true;
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

    if (options.list == false && strlen(options.read) == 0 && options.readall == false) {
        fprintf(stderr, "Either -l (--list) or -r (--read) must be used\n");

        return EXIT_FAILURE;
    }

    return capabilities_run(&options);

}