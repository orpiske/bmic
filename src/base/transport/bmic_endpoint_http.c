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

typedef struct bmic_reply_data_t_
{
    bmic_data_t *body;
    gru_status_t *status;
} bmic_reply_data_t;

static inline CURL *bmic_curl_easy(bmic_endpoint_t *ep)
{
    return (CURL *) ep->handle;
}

static size_t curl_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    bmic_reply_data_t *p = (struct bmic_reply_data_t *) userp;

    p->body->data = (char *) realloc(p->body->data, p->body->size + realsize + 1);

    if (p->body->data == NULL) {
        gru_status_set(p->status, GRU_FAILURE, 
                       "Unable to allocate memory for reply data");

        free(p->body->data);
        return -1;
    }


    memcpy(&(p->body->data[p->body->size]), contents, realsize);
    p->body->size += realsize;
    ((char *) p->body->data)[p->body->size] = 0;

    return realsize;
}

void bmic_endpoint_http_begin(bmic_endpoint_t *ep, gru_status_t *status) {
    CURL *easy = NULL;

    easy = curl_easy_init();
    if (!easy) {
        gru_status_set(status, GRU_FAILURE, "Unable to initialize CURL");

        return;
    }
    
    gru_status_reset(status);
    ep->handle = easy;
}

void bmic_endpoint_http_terminate(bmic_endpoint_t *ep, gru_status_t *status) {
    CURL *easy = bmic_curl_easy(ep);

    curl_easy_cleanup(easy);
    gru_status_reset(status);
    
    ep->handle = NULL;
}


// HTTP GET only
void bmic_endpoint_http_read(bmic_endpoint_t *ep, bmic_data_t *payload,
                             bmic_data_t *data, gru_status_t *status)
{
    CURL *easy = bmic_curl_easy(ep);
    
    curl_easy_setopt(easy, CURLOPT_URL, ep->url);

    bmic_reply_data_t reply = {0};
    reply.body = data;

    if (ep->credentials->username != NULL) {
        curl_easy_setopt(easy, CURLOPT_USERNAME, ep->credentials->username);
    }

    if (ep->credentials->password != NULL) {
        curl_easy_setopt(easy, CURLOPT_PASSWORD, ep->credentials->password);
    }


    curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, curl_callback);
    curl_easy_setopt(easy, CURLOPT_WRITEDATA, &reply);
    curl_easy_setopt(easy, CURLOPT_USERAGENT, "bmic/0.0.1");

    curl_easy_setopt(easy, CURLOPT_TIMEOUT, 5);

    curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, 1);

    curl_easy_setopt(easy, CURLOPT_MAXREDIRS, 3);

    CURLcode rcode = curl_easy_perform(easy);

    return rcode;
}

// POST + get reply
void bmic_endpoint_http_write(bmic_endpoint_t *ep, bmic_data_t *payload,
                              bmic_data_t *data, gru_status_t *status)
{
    CURL *easy = bmic_curl_easy(ep);

    curl_easy_setopt(easy, CURLOPT_URL, ep->url);

    bmic_reply_data_t reply = {0};
    reply.body = data;

    if (ep->credentials->username != NULL) {
        curl_easy_setopt(easy, CURLOPT_USERNAME, ep->credentials->username);
    }

    if (ep->credentials->password != NULL) {
        curl_easy_setopt(easy, CURLOPT_PASSWORD, ep->credentials->password);
    }
    
    struct curl_slist *headers = NULL;

    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(easy, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(easy, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(easy, CURLOPT_POSTFIELDS, bmic_data_to_string(data));

    curl_easy_setopt(easy, CURLOPT_WRITEDATA, &reply);
    curl_easy_setopt(easy, CURLOPT_USERAGENT, "bmic/0.0.1");

    curl_easy_setopt(easy, CURLOPT_TIMEOUT, 5);

    curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, 1);

    curl_easy_setopt(easy, CURLOPT_MAXREDIRS, 3);

    CURLcode rcode = curl_easy_perform(easy);

    return rcode;
}