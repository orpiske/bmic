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
#include "bmic_artemis_json.h"
#include "bmic_artemis_mi.h"

/*
 curl -u admin:admin -H "Content-Type: application/json" -X POST 
 *  -d '{ 
 *       "type":"EXEC", 
 *        "mbean":"org.apache.activemq.artemis:brokerName=\"0.0.0.0\",module=Core,serviceType=Server,type=Broker", 
 *        "operation": "createQueue(java.lang.String,java.lang.String)", 
 *        "arguments": ["name", "address"] 
 *      }'
 * 
 *      http://localhost:8161/jolokia/
 */

void bmic_artemis_json_create_queue(const bmic_object_t *op, bmic_json_t *json, 
                                    const char *name) 
{
    json_object *op_type = json_object_new_string("EXEC");
    json_object_object_add(json->obj, "type", op_type);
    
    char *mbean_value = NULL;
    asprintf(&mbean_value, "%s:%s", ARTEMIS_BASE_PKG, op->name);
    json_object *mbean = json_object_new_string(mbean_value);
    free(mbean_value);
    
    json_object_object_add(json->obj, "mbean", mbean);
    
    json_object *create_queue = json_object_new_string(CREATE_CORE_QUEUE_SIG);
    json_object_object_add(json->obj, "operation", create_queue);
    
    // Arguments
    json_object *arguments = json_object_new_array();
    json_object *arg_name = json_object_new_string(name);
    json_object *address = json_object_new_string(name);
    json_object_array_add(arguments, arg_name);
    json_object_array_add(arguments, address);
    json_object_object_add(json->obj, "arguments", arguments);
    
}