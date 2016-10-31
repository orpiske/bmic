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

#include <transport/bmic_endpoint.h>
#include <transport/bmic_endpoint_http.h>

#include <common/gru_status.h>

int main(int argc, char** argv) {
    bmic_endpoint_t ep;
    gru_status_t status = {0};

    ep.username = (char *) argv[1];
    ep.password = (char *) argv[2];
    ep.url = (char *) argv[3];
    bmic_data_t reply = {0};

    bmic_endpoint_http_read(&ep, NULL, &reply, &status);
    printf("%s\n", reply.data);

    return (EXIT_SUCCESS);
}

