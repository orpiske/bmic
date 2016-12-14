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
#include "bmic_opt_common.h"

options_t prg_options = {0};

options_t options_init(program_type_t type)
{
    prg_options.type = type;

    atexit(options_cleanup);

    switch (prg_options.type) {
    case CAPABILITIES:
    {
        prg_options.program.capabilities.list = false;
        prg_options.program.capabilities.readall = false;
        prg_options.program.capabilities.show_info = false;

        break;
    }
    case OPERATIONS:
    {
        prg_options.program.operations.show_info = false;

        break;
    }
    case QUEUE:
    {
        prg_options.program.queue.show_info = false;

        break;
    }
    case TOP:
    {
        prg_options.program.top.interval = 5;
        prg_options.program.top.repeat = -1;
        break;
    }
    default: break;
    }


    return prg_options;
}

void options_cleanup()
{
    if (prg_options.credentials.username) {
        gru_dealloc_string(&prg_options.credentials.username);
    }

    if (prg_options.credentials.password) {
        gru_dealloc_string(&prg_options.credentials.password);
    }

    switch (prg_options.type) {
    case CAPABILITIES:
    {
        if (prg_options.program.capabilities.read) {
            gru_dealloc_string(&prg_options.program.capabilities.read);
        }

        break;
    }
    case OPERATIONS:
    {
        if (prg_options.program.operations.name) {
            gru_dealloc_string(&prg_options.program.operations.name);
        }

        break;
    }
    case QUEUE:
    {
        if (prg_options.program.queue.attribute) {
            gru_dealloc_string(&prg_options.program.queue.attribute);
        }

        if (prg_options.program.queue.queue) {
            gru_dealloc_string(&prg_options.program.queue.queue);
        }

        break;


    }
    case TOP:
    default:
    {
        break;
    }
    }
}

void print_option_help(const char *long_opt, const char *short_opt, const char *desc)
{
    printf("\t%s%s--%s (-%s short option) %s\n\t\t%s\n\n", RESET, LIGHT_WHITE, long_opt,
           short_opt, RESET, desc);
}

void print_program_usage(char *program_name)
{
    printf("%s%sUsage:%s\nbmic %s <options>\n\nValid options:\n\n", RESET, LIGHT_WHITE, RESET, program_name);
}