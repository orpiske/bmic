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
#ifndef BMIC_JOLOKIA_H
#define BMIC_JOLOKIA_H

#include <common/gru_status.h>

#include "base/common/bmic_object.h"
#include "base/common/bmic_cap_info.h"
#include "base/common/bmic_op_arg.h"
#include "base/common/bmic_op_sig.h"
#include "base/common/bmic_op_info.h"

#include "management/common/bmic_internal_payloads.h"

#include "bmic_jolokia_status.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Reads the information about a capability or attribute matching the given name
 * @param capabilities A top-level bmic_objec_t that contains the details of the 
 * capability to be read (ie.: usually one of the child-elements of the root object)
 * @param attr_name the name of the attribute or capability to read
 * @param status Status code/message
 * @return the Attribute or capability information or NULL in case of error (including 
 * not found)
 */
const bmic_cap_info_t *bmic_jolokia_read_attr_info(const bmic_object_t *capabilities, 
                                                            const char *attr_name,
                                                            gru_status_t *status);
    
/**
 * Given an object of attributes, read them into the info object
 * @param obj
 * @param info
 * @note The expected formated for the transformed BMIC Object is: 
 *    "attr": { "name": { "rw": true, "type": "boolean", "desc": "description" },
 */
void bmic_jolokia_translate_attr(const bmic_object_t *obj,
                                         bmic_cap_info_t *info);



/**
 * Given a list of bmic_objects representing Jolokia attributes, translates them into a 
 * list of attributes. This method is meant to be used when iterating over a list of 
 * objects, therefore it must comply with the tree_callback_fn signature
 * @param nodedata The node data (which is a bmic_object_t *) 
 * @param payload A payload wrapper of type bmic_payload_add_attr_t  
 */
void bmic_jolokia_translate_attr_object(const void *nodedata, void *payload);

/**
 * Given an object that represents a single-signature operation or a multi-signature 
 * operation, reads it and transform into a bmic_op_info_t object.
 * @param obj
 * @param info
 * @note The expected formated for the transformed BMIC Object is: 
 *  "opname": { "args": [ { "name": "p1", "type": "java.lang.String", "desc": "" } }
 */
void bmic_jolokia_translate_op(const bmic_object_t *obj,
                                         bmic_op_info_t *info,
                                        gru_status_t *status);

/**
 * Given a list of bmic_objects representing Jolokia operations, translates them into a 
 * list of operations. This method is meant to be used when iterating over a list of 
 * objects, therefore it must comply with the tree_callback_fn signature
 * @param nodedata The node data (which is a bmic_object_t *) 
 * @param payload A payload wrapper of type bmic_payload_add_attr_t  
 */
void bmic_jolokia_translate_op_object(const void *nodedata, void *payload);

/**
 * Reads an object tree and tries to find the status as defined by Jolokia API
 * @param root
 * @param status
 * @return 
 */
bool bmic_jolokia_translate_status(bmic_object_t *root, gru_status_t *status);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_JOLOKIA_H */

