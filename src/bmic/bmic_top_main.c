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

#define as_mb(x) (x / (1024*1024))

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

static void print_queue_stat(const char *name, bmic_queue_stat_t stat)
{
    printf("%-40s %-10"PRId64" %-10"PRId64" %-10"PRId64" %-10"PRId64"\n", name, 
           stat.queue_size, stat.consumer_count, stat.msg_ack_count, stat.msg_exp_count);
}

static void print_mem(const char *name, bmic_java_mem_info_t *mem) {
    printf("%-20s %-15"PRId64" %-15"PRId64" %-15"PRId64" %-15"PRId64"\n", name, 
           as_mb(mem->init), 
           as_mb(mem->committed), as_mb(mem->max), as_mb(mem->used));
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
    
    const bmic_exchange_t *cap = api->load_capabilities(ctxt.handle, &status);
    if (!cap) {
        fprintf(stderr, "Unable to load capabilities: %s\n", status.message);

        return EXIT_FAILURE;
    }
    
    const bmic_list_t *list = api->list_queues(ctxt.handle, cap, &status);
    
    bmic_java_info_t jinfo = api->java.java_info(ctxt.handle, &status);
    
    while (true) {
        
        bmic_java_os_info_t osinfo = api->java.os_info(ctxt.handle, &status);
        
        printf(CLEAR_SCREEN);
        printf("%s %s (%s) %s %s \n", jinfo.name, jinfo.version, jinfo.jvm_package_version, 
               osinfo.name, 
               osinfo.version);        
        
        printf("%s%sLoad average:%s %-10.1f\n", RESET, LIGHT_WHITE, RESET, 
               osinfo.load_average);
        printf("%s%sFile descriptors:%s %10lu max total %10lu open %10lu free\n", 
               RESET, LIGHT_WHITE, RESET, 
               osinfo.max_fd, osinfo.open_fd, (osinfo.max_fd - osinfo.open_fd));
        printf("%s%sPhysical memory:%s %10"PRId64" total %10"PRId64" free\n", RESET, LIGHT_WHITE, 
               RESET, 
               as_mb(osinfo.mem_total), as_mb(osinfo.mem_free));
        printf("%s%sSwap memory:%s %10"PRId64" total %10"PRId64" free %10"PRId64" used\n\n", 
               RESET, LIGHT_WHITE, RESET, as_mb(osinfo.swap_total), as_mb(osinfo.swap_free), 
               as_mb(osinfo.swap_committed));
        
        printf("%s%s%s%-20s %-15s %-15s %-15s %-15s%s\n", RESET, BG_WHITE, LIGHT_BLACK,
               "Area", "Initial", "Committed", "Max", "Used", RESET);
        bmic_java_mem_info_t eden = api->java.eden_info(ctxt.handle, &status);
        print_mem("Eden", &eden);
        
        bmic_java_mem_info_t survivor = api->java.survivor_info(ctxt.handle, &status);
        print_mem("Survivor", &survivor);
        
        bmic_java_mem_info_t tenured = api->java.tenured_info(ctxt.handle, &status);
        print_mem("Tenured", &tenured);
        
        if (jinfo.memory_model == JAVA_MODERN) {
            bmic_java_mem_info_t metaspace = api->java.metaspace_info(ctxt.handle, &status);
            print_mem("Metaspace", &metaspace);
        }
        else {
            bmic_java_mem_info_t permgen = api->java.permgen_info(ctxt.handle, &status);
            print_mem("PermGen", &permgen);
        }
        
        if (list && list->items) {
            printf("\n\n");
            printf("%s%s%s%-40s %-10s %-10s %-10s %-10s%s\n", RESET, BG_WHITE, LIGHT_BLACK, 
                   "Name", "Size", "Consumers", "Ack Count", "Exp Count", RESET);
            
            for (uint32_t i = 0; i < gru_list_count(list->items); i++) {
                const gru_node_t *node = gru_list_get(list->items, i);
                
                if (node != NULL && node->data != NULL) { 
                    
                    bmic_queue_stat_t stat = 
                            api->stat_queue(ctxt.handle, cap, (const char *) node->data, 
                                            &status);

                    print_queue_stat((const char *) node->data, stat);
                }
            }
        }
        
        sleep(5);
        
    }

    bmic_list_destroy(&list);
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
            strlcpy(options.username, optarg, sizeof (options.username));
            break;
        case 'p':
            strlcpy(options.password, optarg, sizeof (options.password));
            break;
        case 's':
            strlcpy(options.server, optarg, sizeof (options.server));
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