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
#include "bmic_discovery_hint.h"

bmic_discovery_hint_t *bmic_discovery_hint_new(gru_status_t *status) {
	bmic_discovery_hint_t *ret = gru_alloc(sizeof(bmic_discovery_hint_t), status);
	gru_alloc_check(ret, NULL);

	return ret;
}

void bmic_discovery_hint_set_url(
	bmic_discovery_hint_t *hint, const char *url, gru_status_t *status) {
	gru_dealloc_string(&hint->content.url);

	hint->hint_type = BMIC_URL;

	if (asprintf(&hint->content.url, "%s", url) == -1) {
		gru_status_set(status, GRU_FAILURE, "Unable to eval URL: not enough memory");

		return;
	}
}

void bmic_discovery_hint_set_addressing_hostname(
	bmic_discovery_hint_t *hint, const char *hostname, gru_status_t *status) {
	gru_dealloc_string(&hint->content.addressing.hostname);

	hint->hint_type = BMIC_ADDRESSING;
	if (hostname == NULL) {
		if (asprintf(&hint->content.addressing.hostname, "%s", "localhost") == -1) {
			gru_status_set(
				status, GRU_FAILURE, "Unable to set addressing: not enough memory");
		}
	} else {
		if (asprintf(&hint->content.addressing.hostname, "%s", hostname) == -1) {
			gru_status_set(
				status, GRU_FAILURE, "Unable to set addressing: not enough memory");
		}
	}
}

void bmic_discovery_hint_set_addressing_port(
	bmic_discovery_hint_t *hint, uint16_t port, gru_status_t *status) {
	hint->content.addressing.port = port;
}

bmic_discovery_hint_t *bmic_discovery_hint_eval_addressing(
	const char *hostname, uint16_t port, gru_status_t *status) {
	bmic_discovery_hint_t *ret = bmic_discovery_hint_new(status);
	gru_alloc_check(ret, NULL);

	ret->hint_type = BMIC_ADDRESSING;
	if (hostname == NULL) {
		if (asprintf(&ret->content.addressing.hostname, "%s", "localhost") == -1) {
			gru_status_set(
				status, GRU_FAILURE, "Unable to set addressing: not enough memory");

			bmic_discovery_hint_destroy(&ret);

			return NULL;
		}
	} else {
		if (asprintf(&ret->content.addressing.hostname, "%s", hostname) == -1) {
			gru_status_set(
				status, GRU_FAILURE, "Unable to set addressing: not enough memory");

			bmic_discovery_hint_destroy(&ret);

			return NULL;
		}
	}

	ret->content.addressing.port = port;
	return ret;
}

bmic_discovery_hint_t *bmic_discovery_hint_eval_url(
	const char *url, gru_status_t *status) {
	bmic_discovery_hint_t *ret = gru_alloc(sizeof(bmic_discovery_hint_t), status);
	gru_alloc_check(ret, NULL);

	if (url == NULL) {
		return bmic_discovery_hint_eval_addressing(NULL, BMIC_PORT_UNKNOWN, status);
	}

	ret->hint_type = BMIC_URL;
	if (asprintf(&ret->content.url, "%s", url) == -1) {
		gru_status_set(status, GRU_FAILURE, "Unable to eval URL: not enough memory");

		bmic_discovery_hint_destroy(&ret);

		return NULL;
	}

	return ret;
}

void bmic_discovery_hint_destroy(bmic_discovery_hint_t **hint) {
	bmic_discovery_hint_t *h = *hint;

	if (h == NULL) {
		return;
	}

	if (h->hint_type == BMIC_ADDRESSING) {
		free(h->content.addressing.hostname);
	} else {
		if (h->hint_type == BMIC_URL) {
			free(h->content.url);
		}
	}

	gru_dealloc((void **) hint);
}

const char *bmic_discovery_hint_to_url(const bmic_discovery_hint_t *hint,
	const char *url_format, const char *addr_format, uint64_t default_port) {
	logger_t logger = gru_logger_get();
	char *ret = NULL;

	if (hint->hint_type == BMIC_URL) {
		if (url_format == NULL) {
			logger(
				FATAL, "Unable to format URL based on hint because URL format is NULL");

			return NULL;
		}

		if (asprintf(&ret, url_format, hint->content.url) == -1) {
			logger(FATAL, "Not enough memory to set URL hint");

			return NULL;
		}
	} else {
		if (addr_format == NULL) {
			logger(FATAL, "Unable to format URL based on hint "
						  "because addressing format is NULL");

			return NULL;
		}
		uint16_t port = (hint->content.addressing.port == BMIC_PORT_UNKNOWN
				? default_port
				: hint->content.addressing.port);

		if (asprintf(&ret, addr_format, hint->content.addressing.hostname, port) == -1) {
			logger(FATAL, "Not enough memory to set addressing hint");

			return NULL;
		}
	}

	return ret;
}

const char *bmic_discovery_hint_host(const bmic_discovery_hint_t *hint) {
	if (hint->hint_type == BMIC_ADDRESSING) {
		return hint->content.addressing.hostname;
	} else {
		return hint->content.url;
	}
}