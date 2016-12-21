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

#ifndef BMIC_DISCOVERY_HINT_H
#define BMIC_DISCOVERY_HINT_H

#include <stdint.h>

#include <common/gru_alloc.h>
#include <common/gru_status.h>
#include <log/gru_logger.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BMIC_PORT_UNKNOWN 0

typedef enum bmic_hint_type_t_ {
	BMIC_ADDRESSING,
	BMIC_URL,
} bmic_hint_type_t;

typedef struct bmic_addressing_t_ {
	char *hostname;
	uint16_t port;
} bmic_addressing_t;

typedef struct bmic_discovery_hint_t_ {
	bmic_hint_type_t hint_type;

	union {
		bmic_addressing_t addressing;
		char *url;
	} content;

} bmic_discovery_hint_t;

/**
 * Creates a new discovery hint object
 * @param status
 * @return
 */
bmic_discovery_hint_t *bmic_discovery_hint_new(gru_status_t *status);

/**
 * Sets the URL for the discovery hint
 * @param hint
 * @param url
 * @param status
 */
void bmic_discovery_hint_set_url(
	bmic_discovery_hint_t *hint, const char *url, gru_status_t *status);

/**
 * Sets the hostname for the discovery hint
 * @param hint
 * @param url
 * @param status
 */
void bmic_discovery_hint_set_addressing_hostname(
	bmic_discovery_hint_t *hint, const char *hostname, gru_status_t *status);

/**
 * Sets the portfor the discovery hint
 * @param hint
 * @param url
 * @param status
 */
void bmic_discovery_hint_set_addressing_port(
	bmic_discovery_hint_t *hint, uint16_t port, gru_status_t *status);

/**
 * Gets the host/URL for the discovery hint
 * @param hint
 * @param url
 * @param status
 */
const char *bmic_discovery_hint_host(const bmic_discovery_hint_t *hint);

/**
 * Evaluates input parameters to build a hint structure used to figure out broker
 * parameters
 * @param hostname The hostname. If NULL is given, defaults to localhost
 * @param port The port of BMIC_PORT_UNKNOWN if unknown
 * @param status A status struct that will contain error data if the function failed
 * @return A hint structure that can be used to find out details about the broker
 */
bmic_discovery_hint_t *bmic_discovery_hint_eval_addressing(
	const char *hostname, uint16_t port, gru_status_t *status);

/**
 * Evaluates input parameters to build a hint structure used to figure out broker
 * parameters
 * @param url The management URL. If null, will use defaults from
 * bmic_discovery_hint_eval_addressing
 * @param status A status struct that will contain error data if the function failed
 * @return A hint structure that can be used to find out details about the broker
 */
bmic_discovery_hint_t *bmic_discovery_hint_eval_url(
	const char *url, gru_status_t *status);

/**
 * Destroy the hint structure and frees all memory used by it
 * @param hint
 */
void bmic_discovery_hint_destroy(bmic_discovery_hint_t **hint);

/**
 * Given an URL format specification or a an address format specification, returns a URL
 * @param hint URL and or addressing hint
 * @param url_format URL format specification (ie.: http://%s:%i/api/jolokia)
 * @param addr_format addressing format specification (ie: %s/api/jolokia)
 * @param default_port default port for the service or BMIC_PORT_UNKNOWN
 * @return The URL as a newly allocated char string that must be free'd
 */
const char *bmic_discovery_hint_to_url(const bmic_discovery_hint_t *hint,
	const char *url_format, const char *addr_format, uint64_t default_port);

#ifdef __cplusplus
}
#endif

#endif /* BMIC_DISCOVERY_HINT_H */
