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
#ifndef BMIC_ARTEMIS_DEFS_H
#define BMIC_ARTEMIS_DEFS_H

#define ARTEMIS_PRODUCT_NAME "artemis"
#define ARTEMIS_PRODUCT_NAME_PRETTY "Artemis"
#define ARTEMIS_API_VERSION "1.x.x"

/**
 * Base URLs must not have the '/' char at the end
 */
#define ARTEMIS_BASE_URL_HINT_URL "%s/jolokia"
#define ARTEMIS_BASE_URL_HINT_ADDRESSING "http://%s:%i/jolokia"
#define ARTEMIS_PRODUCT_INFO_PATH                                                        \
	"read/"                                                                              \
	"org.apache.activemq.artemis:brokerName=\"0.0.0.0\",module=Core,serviceType=Server," \
	"type=Broker/Version"
#define ARTEMIS_PRODUCT_CAPABILITIES "list/org.apache.activemq.artemis"

// Artemis 2.0.0 and beyond have a different path
#define ARTEMIS_CAPABILITIES_KEY_V20_REGEX "(broker=)([^,]*)$"

#define ARTEMIS_CORE_CAP_ATTRIBUTES	"(broker=)([^,]*)\\/attr$"

#define ARTEMIS_QUEUE_CAPABILITIES_V2_REGEX "(.*,queue=\"%s\".*)"

#define ARTEMIS_CORE_CAP_OPERATIONS "(broker=)([^,]*)\\/op$"

#define ARTEMIS_QUEUE_LIST_ATTR "QueueNames"
#define ARTEMIS_QUEUE_SIZE_ATTR "MessageCount"
#define ARTEMIS_QUEUE_ACK_CNT_ATTR "MessagesAcknowledged"
#define ARTEMIS_QUEUE_EXP_CNT_ATTR "MessagesExpired"
#define ARTEMIS_QUEUE_CNS_CNT_ATTR "ConsumerCount"

#endif /* BMIC_ARTEMIS_DEFS_H */
