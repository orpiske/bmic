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
#include "bmic_artemis.h"
#include "base/format/bmic_json.h"


bmic_api_interface_t *bmic_artemis_product(gru_status_t *status) {
    bmic_api_interface_t *ret = bmic_api_interface_new("artemis", "1.x.x", status);

    ret->base_url = bmic_artemis_base_url;
    ret->api_init = bmic_artemis_init;
    ret->product_info = bmic_artemis_product_info;
    ret->api_cleanup = bmic_artemis_cleanup;

    return ret;
}

const char *bmic_artemis_base_url(const bmic_discovery_hint_t *hint) {
    char *ret = NULL;

    if (hint->hint_type == URL) {
        asprintf(&ret, ARTEMIS_BASE_URL_HINT_URL,
                 hint->content.url);
    }
    else {
        asprintf(&ret, ARTEMIS_BASE_URL_HINT_ADDRESSING,
                 hint->content.addressing.hostname, 8161);
    }

    return ret;
}

bmic_handle_t *bmic_artemis_init(const char *base_url,
                                 bmic_credentials_t *credentials,
                                 gru_status_t *status) {
    bmic_handle_t *handle = NULL;

    handle = gru_alloc(sizeof(bmic_handle_t), status);
    gru_alloc_check(handle, NULL);

    handle->ep = bmic_endpoint_init(base_url, NULL, NULL, status);
    if (handle->ep == NULL) {
        bmic_artemis_cleanup(&handle);

        return NULL;
    }

    bmic_endpoint_set_credentials(handle->ep, credentials, status);
    bmic_endpoint_http_begin(handle->ep, status);

    handle->transport.read = bmic_endpoint_http_read;
    handle->transport.write = bmic_endpoint_http_write;

    return handle;
}


void bmic_artemis_cleanup(bmic_handle_t **handle) {
    bmic_handle_destroy(handle, bmic_endpoint_http_terminate);
}

bmic_product_info_t *bmic_artemis_product_info(bmic_handle_t *handle,
        gru_status_t *status)
{
    bmic_data_t reply = {0};
    bmic_api_io_read(handle, ARTEMIS_PRODUCT_INFO_PATH, &reply, status);

    if (status->code != GRU_SUCCESS) {
        return NULL;
    }

    bmic_json_value_t value = {0};
    bmic_api_io_find_value(&reply, &value, "value", status);

    if (value.type == STRING) {
        printf("Version: %s\n", value.data.str);

        bmic_product_info_t *ret = gru_alloc(sizeof(bmic_api_interface_t), status);
        snprintf(ret->version, sizeof(ret->version), "%s", value.data.str);

        return ret;
    }

    return NULL;
}
