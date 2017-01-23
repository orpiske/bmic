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
#ifndef BMIC_QUEUE_STAT_H
#define BMIC_QUEUE_STAT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bmic_queue_stat_t_ {
	int64_t consumer_count;
	int64_t msg_ack_count;
	int64_t msg_exp_count;
	int64_t queue_size;
} bmic_queue_stat_t;

typedef struct bmic_topic_stat_t_ {
	int64_t consumer_count;
	int64_t msg_enq_count;
	int64_t msg_deq_count;
	int64_t queue_size;
} bmic_topic_stat_t;

#ifdef __cplusplus
}
#endif

#endif /* BMIC_QUEUE_STAT_H */
