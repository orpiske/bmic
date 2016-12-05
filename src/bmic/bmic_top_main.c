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
#include "bmic_top_main.h"

typedef struct options_t_
{
    char username[OPT_MAX_STR_SIZE];
    char password[OPT_MAX_STR_SIZE];
    char server[OPT_MAX_STR_SIZE];
} options_t;

static void show_help()
{
    printf("Usage: ");
    printf("\t-h\t--help show this help\n");
    printf("\t-u\t--username=<str> username to access the management console\n");
    printf("\t-p\t--password=<str> password to access the management console\n");
    printf("\t-s\t--server=<str> hostname or IP address of the server\n");
}

/*
 typedef struct bmic_java_mem_info_t_ {
    uint64_t init;
    uint64_t committed;
    uint64_t max;
    uint64_t used;
} bmic_java_mem_info_t;
 */

static void print_mem(const char *name, bmic_java_mem_info_t *mem) {
    printf("%s memory initial: %lu\n", name, mem->init);
    printf("%s memory committed: %lu\n", name, mem->committed);
    printf("%s memory max: %lu\n", name, mem->max);
    printf("%s memory used: %lu\n", name, mem->used);
}

int top_run(options_t *options)
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
    
    while (true) {
        bmic_java_mem_info_t eden = api->java.eden_info(ctxt.handle, &status);
        print_mem("Eden", &eden);
        
        bmic_java_mem_info_t survivor = api->java.survivor_info(ctxt.handle, &status);
        print_mem("Survivor", &survivor);
        
        printf("\n");
        sleep(5);
    }

    bmic_context_cleanup(&ctxt);

    return EXIT_SUCCESS;
}

int top_main(int argc, char **argv) {
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
            strncpy(options.username, optarg, sizeof (options.username) - 1);
            break;
        case 'p':
            strncpy(options.password, optarg, sizeof (options.password) - 1);
            break;
        case 's':
            strncpy(options.server, optarg, sizeof (options.server) - 1);
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

    return top_run(&options);
}