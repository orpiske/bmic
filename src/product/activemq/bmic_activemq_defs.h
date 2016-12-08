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

#ifndef BMIC_ACTIVEMQ_DEFS_H
#define BMIC_ACTIVEMQ_DEFS_H

#define ACTIVEMQ_PRODUCT_NAME "activemq"
#define ACTIVEMQ_API_VERSION "5.x.x"

/**
 * Base URLs must not have the '/' char at the end
 */
#define ACTIVEMQ_BASE_URL_HINT_URL "%s/api/jolokia"
#define ACTIVEMQ_BASE_URL_HINT_ADDRESSING "http://%s:%i/api/jolokia"
#define ACTIVEMQ_PRODUCT_INFO_PATH "read/org.apache.activemq:brokerName=localhost,type=Broker/BrokerVersion"
#define ACTIVEMQ_PRODUCT_CAPABILITIES "list/org.apache.activemq"

#define ACTIVEMQ_CAPABILITIES_KEY_REGEX "(brokerName=)([a-z0-9\\.])*(,type=Broker)"
#define ACTIVEMQ_CORE_CAP_ATTRIBUTES "(brokerName=)([a-z0-9\\.])*(,type=Broker\\/attr$)"

#define ACTIVEMQ_QUEUE_CAPABILITES_REGEX "(brokerName=)([a-zA-Z0-9\\.\"]*)(,destinationName=)(%s)(,destinationType=)([a-zA-Z0-9\\.\"]*)(,type=)([a-zA-Z0-9\\.\"]*)"

#define ACTIVEMQ_CAP_OPERATIONS "(brokerName=)([a-z0-9\\.\"])*(,type=Broker\\/op$)"
#define ACTIVEMQ_BROKER_OPERATIONS_ROOT "(brokerName=)([a-z0-9\\.\"])*(,type=Broker$)"


#define ACTIVEMQ_QUEUE_SIZE_ATTR "QueueSize"
#define ACTIVEMQ_QUEUE_ACK_CNT_ATTR "DequeueCount"
#define ACTIVEMQ_QUEUE_EXP_CNT_ATTR "ExpiredCount"
#define ACTIVEMQ_QUEUE_CNS_CNT_ATTR "ConsumerCount"

#endif /* BMIC_ACTIVEMQ_DEFS_H */

