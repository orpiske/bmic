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
#include <stdio.h>
#include <stdlib.h>

#include <common/gru_status.h>

#include "bmic_capabilities_main.h"
#include "bmic_discovery_main.h"
#include "bmic_queue_main.h"
#include "bmic_operations_main.h"


void show_help() {
    printf("Usage: \n");
    printf("\t\tdiscovery\tRun a discovery on the broker to find its type and version\n");
    printf("\t\tcapabilities\tRead/write/list broker capabilities and attributes\n");
    printf("\t\tqueue\tRead/write/list queue capabilities and attributes\n");
}
int main(int argc, char** argv)
{
    
     if (argc < 2) {
        show_help();
        
        return EXIT_FAILURE;
    }
    else {
        if (strcmp(argv[1], "capabilities") == 0) {
            return capabilities_main((argc - 1), &argv[1]);
        }
        
        if (strcmp(argv[1], "discovery") == 0) {
            return discovery_main((argc - 1), &argv[1]);
        }
        
        if (strcmp(argv[1], "queue") == 0) {
            return queue_main((argc - 1), &argv[1]);
        }
        
        if (strcmp(argv[1], "operations") == 0) {
            return operations_main((argc - 1), &argv[1]);
        }
    }
    
    return EXIT_SUCCESS;
}

