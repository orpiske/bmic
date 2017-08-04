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
#include "bmic_endpoint_http.h"
#include "bmic_endpoint.h"

typedef struct bmic_reply_data_t_ {
	bmic_data_t *body;
	gru_status_t *status;
} bmic_reply_data_t;

static inline CURL *bmic_curl_easy(bmic_endpoint_t *ep) {
	return (CURL *) ep->handle;
}

static inline CURL *bmic_curl_easy_const(const bmic_endpoint_t *ep) {
	return (CURL *) ep->handle;
}

static size_t curl_callback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;
	bmic_reply_data_t *p = (bmic_reply_data_t *) userp;
	logger_t logger = gru_logger_get();

	logger(GRU_TRACE, "Reading data: %s", (char *) contents);

	p->body->data = (char *) realloc(p->body->data, p->body->size + realsize + 1);

	if (p->body->data == NULL) {
		gru_status_set(
			p->status, GRU_FAILURE, "Unable to allocate memory for reply data");

		free(p->body->data);
		return 0;
	}

	memcpy(&(((char *) p->body->data)[p->body->size]), contents, realsize);
	p->body->size += realsize;
	((char *) p->body->data)[p->body->size] = 0;

	return realsize;
}

void bmic_endpoint_http_begin(bmic_endpoint_t *ep, gru_status_t *status) {
	CURL *easy = NULL;
	static bool initialized = false;

	logger_t logger = gru_logger_get();

	if (!initialized) {
		logger(GRU_DEBUG, "Initializing global curl data");

		if (strstr(ep->url, "https")) {
			curl_global_init(CURL_GLOBAL_DEFAULT);
		} else {
			curl_global_init(CURL_GLOBAL_NOTHING);
		}

		initialized = true;

		atexit(curl_global_cleanup);
	} else {
		logger(GRU_DEBUG, "Curl global data already initialized ... skipping");
	}

	easy = curl_easy_init();
	if (!easy) {
		gru_status_set(status, GRU_FAILURE, "Unable to initialize CURL");

		return;
	}

	gru_status_reset(status);
	ep->handle = easy;
	ep->options.timeout = 35L;
}

void bmic_endpoint_http_terminate(bmic_endpoint_t *ep, gru_status_t *status) {
	CURL *easy = bmic_curl_easy(ep);

	curl_easy_cleanup(easy);

	gru_status_reset(status);

	ep->handle = NULL;
}

static char *bmic_endpoint_http_path_join(
	const bmic_endpoint_t *ep, CURL *easy, gru_status_t *status) {
	char *full_url = NULL;

	int ret = 0;
	if (ep->path) {
		char *escaped_path = curl_easy_escape(easy, ep->path, 0);
		ret = asprintf(&full_url, "%s/%s", ep->url, escaped_path);

		curl_free(escaped_path);
	} else {
		ret = asprintf(&full_url, "%s", ep->url);
	}

	if (ret == -1) {
		gru_status_set(status, GRU_FAILURE, "Not enough memory to join URL path");

		return NULL;
	}

	return full_url;
}

static void bmic_endpoint_http_path_cleanup(char **path) {
	gru_dealloc_string(path);
}

void bmic_endpoint_http_read(const bmic_endpoint_t *ep, const bmic_data_t *request,
	bmic_data_t *reply, bmic_endpoint_status_t *epstatus) {
	assert(ep != NULL);
	logger_t logger = gru_logger_get();

	CURL *easy = bmic_curl_easy_const(ep);
	curl_easy_reset(easy);

	char *full_path = bmic_endpoint_http_path_join(ep, easy, epstatus->status);

	if (full_path == NULL) {
		return;
	}

	logger(GRU_DEBUG, "Sending request to %s", full_path);
	curl_easy_setopt(easy, CURLOPT_URL, full_path);

	bmic_reply_data_t reply_data = {0};
	reply_data.body = reply;

	if (ep->credentials.username != NULL) {
		curl_easy_setopt(easy, CURLOPT_USERNAME, ep->credentials.username);
	}

	if (ep->credentials.password != NULL) {
		curl_easy_setopt(easy, CURLOPT_PASSWORD, ep->credentials.password);
	}

	curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, curl_callback);
	curl_easy_setopt(easy, CURLOPT_WRITEDATA, (void *) &reply_data);
	curl_easy_setopt(easy, CURLOPT_USERAGENT, PROJECT_NAME "/" BMIC_VERSION);

	curl_easy_setopt(easy, CURLOPT_TIMEOUT, ep->options.timeout);
	curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(easy, CURLOPT_MAXREDIRS, 3);

	CURLcode rcode = curl_easy_perform(easy);
	bmic_endpoint_http_path_cleanup(&full_path);

	if (rcode != CURLE_OK) {
		gru_status_set(epstatus->status,
			GRU_FAILURE,
			"Error while trying to read data: %s",
			curl_easy_strerror(rcode));
	} else {
		curl_easy_getinfo(easy, CURLINFO_RESPONSE_CODE, &epstatus->epcode);
		if (epstatus->epcode != HTTP_STATUS_OK) {
			gru_status_set(epstatus->status,
				GRU_FAILURE,
				"Unacceptable response from server (HTTP status %ld)",
				epstatus->epcode);
		}

		logger(GRU_DEBUG, "HTTP response code %d", epstatus->epcode);

		if (reply->size > 0) {
			logger(GRU_TRACE, "HTTP response data %s", bmic_data_to_string(reply_data.body));
		}
	}
}

// POST + get reply

void bmic_endpoint_http_write(const bmic_endpoint_t *ep, const bmic_data_t *request,
	bmic_data_t *reply, bmic_endpoint_status_t *epstatus) {
	assert(ep != NULL);

	CURL *easy = bmic_curl_easy_const(ep);
	curl_easy_reset(easy);

	char *full_path = bmic_endpoint_http_path_join(ep, easy, epstatus->status);
	logger_t logger = gru_logger_get();

	if (full_path == NULL) {
		return;
	}

	curl_easy_setopt(easy, CURLOPT_URL, full_path);
	logger(GRU_DEBUG, "Sending request to %s", full_path);

	bmic_reply_data_t reply_data = {0};
	reply_data.body = reply;

	if (ep->credentials.username != NULL) {
		curl_easy_setopt(easy, CURLOPT_USERNAME, ep->credentials.username);
	}

	if (ep->credentials.password != NULL) {
		curl_easy_setopt(easy, CURLOPT_PASSWORD, ep->credentials.password);
	}

	struct curl_slist *headers = NULL;

	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");

	curl_easy_setopt(easy, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(easy, CURLOPT_HTTPHEADER, headers);

	const char *postdata = bmic_data_to_string(request);
	curl_easy_setopt(easy, CURLOPT_POSTFIELDS, postdata);
	logger(GRU_TRACE, "Sending data %s", postdata);

	curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, curl_callback);
	curl_easy_setopt(easy, CURLOPT_WRITEDATA, (void *) &reply_data);
	curl_easy_setopt(easy, CURLOPT_USERAGENT, PROJECT_NAME "/" BMIC_VERSION);

	curl_easy_setopt(easy, CURLOPT_TIMEOUT, ep->options.timeout);
	curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(easy, CURLOPT_MAXREDIRS, 3);

	CURLcode rcode = curl_easy_perform(easy);
	bmic_endpoint_http_path_cleanup(&full_path);
	curl_slist_free_all(headers);

	if (rcode != CURLE_OK) {
		gru_status_set(epstatus->status,
			GRU_FAILURE,
			"Error while trying to write data: %s",
			curl_easy_strerror(rcode));
	} else {
		curl_easy_getinfo(easy, CURLINFO_RESPONSE_CODE, &epstatus->epcode);
		if (epstatus->epcode != HTTP_STATUS_OK) {
			gru_status_set(epstatus->status,
				GRU_FAILURE,
				"Unacceptable response from server (HTTP status %ld)",
				epstatus->epcode);
		}

		logger(GRU_DEBUG, "HTTP response code %d", epstatus->epcode);

		if (reply->size > 0) {
			logger(GRU_TRACE, "HTTP response data %s", bmic_data_to_string(reply));
		}
	}
}
