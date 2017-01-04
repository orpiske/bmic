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
#include "bmic_activemq_json.h"

/**
 * Sets the mbean part of the exec call
 * @param op
 * @note The first part of exec call is: "type":"EXEC",
 */
static void bmic_activemq_json_op_mbean(const bmic_object_t *op, bmic_json_t *json) {
	char *mbean_value = NULL;
	if (asprintf(&mbean_value, "%s:%s", ACTIVEMQ_BASE_PKG, op->name) == -1) {
		logger_t logger = gru_logger_get();

		logger(FATAL, "Unable to allocate memory for setting the mbean value");

		return;
	}

	json_object *mbean = json_object_new_string(mbean_value);
	free(mbean_value);

	json_object_object_add(json->obj, "mbean", mbean);
}

/**
 * Sets the first part of the exec call
 * @param op
 * @note The first part of exec call is: "type":"EXEC",
 */
static void bmic_activemq_json_op_exec(const bmic_object_t *op, bmic_json_t *json) {
	json_object *op_type = json_object_new_string("EXEC");
	json_object_object_add(json->obj, "type", op_type);
}

/*
 curl -u admin:admin -H "Content-Type: application/json" -X POST
 *  -d '{
 *       "type":"EXEC",
 *
 "mbean":"org.apache.activemq:brokerName=\"0.0.0.0\",module=Core,serviceType=Server,type=Broker",
 *        "operation": "addQueue(java.lang.String,java.lang.String)",
 *        "arguments": ["name", "address"]
 *      }'
 *
 *      http://localhost:8161/jolokia/
 */

void bmic_activemq_json_create_queue(
	const bmic_object_t *op, bmic_json_t *json, const char *name) {
	bmic_activemq_json_op_exec(op, json);

	bmic_activemq_json_op_mbean(op, json);

	json_object *create_queue = json_object_new_string(ACTIVEMQ_CREATE_QUEUE_SIG);
	json_object_object_add(json->obj, "operation", create_queue);

	// Arguments
	json_object *arguments = json_object_new_array();
	json_object *arg_name = json_object_new_string(name);
	json_object_array_add(arguments, arg_name);
	json_object_object_add(json->obj, "arguments", arguments);
}

void bmic_activemq_json_delete_queue(
	const bmic_object_t *op, bmic_json_t *json, const char *name) {
	bmic_activemq_json_op_exec(op, json);

	bmic_activemq_json_op_mbean(op, json);

	json_object *create_queue = json_object_new_string(ACTIVEMQ_REMOVE_QUEUE_SIG);
	json_object_object_add(json->obj, "operation", create_queue);

	// Arguments
	json_object *arguments = json_object_new_array();
	json_object *arg_name = json_object_new_string(name);
	json_object_array_add(arguments, arg_name);
	json_object_object_add(json->obj, "arguments", arguments);
}

void bmic_activemq_json_purge_queue(	const bmic_object_t *op, bmic_json_t *json) {
	bmic_activemq_json_op_exec(op, json);

	bmic_activemq_json_op_mbean(op, json);

	json_object *purge_queue = json_object_new_string(ACTIVEMQ_PURGE_QUEUE_SIG);
	json_object_object_add(json->obj, "operation", purge_queue);

}


void bmic_activemq_json_reset_queue(const bmic_object_t *op, bmic_json_t *json) {
	bmic_activemq_json_op_exec(op, json);

	bmic_activemq_json_op_mbean(op, json);

	json_object *purge_queue = json_object_new_string(ACTIVEMQ_RESET_QUEUE_SIG);
	json_object_object_add(json->obj, "operation", purge_queue);

}