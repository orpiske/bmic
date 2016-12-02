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
#include "bmic_jolokia_io.h"


bool bmic_jolokia_io_exec(bmic_handle_t *handle,
                                            bmic_json_t *json,
                                            gru_status_t *status)
{
    bmic_data_t request = bmic_json_to_data(json, NULL);
    
    bmic_endpoint_status_t epstatus = {
        .status = status,
    };
    
    bmic_endpoint_set_path(handle->ep, "exec");
    
    bmic_data_t reply = {0};
    handle->transport.write(handle->ep, &request, &reply, &epstatus);
    bmic_endpoint_reset_path(handle->ep);
    if (status->code != GRU_SUCCESS) {
        return false;
    }
    
    // TODO: move this to this subcomponent
    bmic_object_t *ret = bmic_jolokia_parse(reply.data, status);
    bmic_data_release(&reply);
    
    if (!bmic_jolokia_translate_status(ret, status)) {
        gru_status_set(status, GRU_FAILURE, "Invalid response from the server: %s", 
                       (reply.data == NULL ? "(null)" : reply.data));
        
        bmic_object_destroy(&ret);
        return false;
    }
    
    bmic_object_destroy(&ret);
    return true;
}