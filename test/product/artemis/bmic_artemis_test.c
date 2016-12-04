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
#include <io/gru_ioutils.h>

#include <base/common/bmic_object.h>
#include <base/format/bmic_json.h>

#include <context/bmic_context.h>

#include <product/artemis/bmic_artemis_mi.h>


static bool open_file(const char *path, bmic_object_t **root, gru_status_t *status) {
    char *buff = NULL;
    
    FILE *file = gru_io_open_file_read_path(path, status);
    
    if (!file || status->code != GRU_SUCCESS) {
        fprintf(stderr, "%s", status->message);
        return EXIT_FAILURE;
    }
    
    size_t size = gru_io_read_text_into(&buff, file, status);
    if (size == 0) {
        fprintf(stderr, "%s", status->message);
        fclose(file);
        
        return EXIT_FAILURE;
    }
    fclose(file);
    
    printf("Read %lu bytes from the file\n", size);
    
    bmic_json_t *json = bmic_json_init(buff, status);
    if (json == NULL) {
        goto err_exit;
    }
    gru_dealloc_string(&buff);
    
    bmic_object_t *ret_root = bmic_object_new_root(status);
    if (!ret_root) {
        bmic_json_destroy(&json);
        goto err_exit;
    }
    
    bmic_json_transform(json, ret_root);
    
    *root = ret_root;
    bmic_json_destroy(&json);
    return true;
    
    err_exit:
    gru_dealloc_string(&buff);
    *root = NULL;
    return false;
}

int main(int argc, char **argv) {
    gru_status_t status = {0};
    bmic_object_t *root = NULL;
    
    if (argc < 3) {
        fprintf(stderr, "Not enough arguments.\nUsage: %s [version] [file]\n", 
                argv[0]);
        
        return EXIT_FAILURE;
    }
    
    printf("Trying to load capability JSON file for Artemis %s\n", argv[1]);
    bmic_log_initialization();
    
    if (!open_file(argv[2], &root, &status)) {
        fprintf(stderr, "Unable to open file");
        
        return EXIT_FAILURE;
    }
    
    
    
    const bmic_object_t *capabilities = bmic_object_find_regex(root,
                                                           ARTEMIS_CAPABILITIES_KEY_REGEX,
                                                           REG_SEARCH_NAME);
    if (capabilities == NULL) {
        bmic_object_destroy(&root);
        fprintf(stderr, "Child not found\n");
        
        goto err_exit;
    }
    
    const bmic_object_t *req_stat = bmic_object_find_by_name(root, "status");
    if (req_stat == NULL) {
        bmic_object_destroy(&root);
        fprintf(stderr, "Child not found\n");
        
        goto err_exit;
    }
    
    
    bmic_object_destroy(&root);
    return EXIT_SUCCESS;
    
    err_exit:
    
    if (status.code != GRU_SUCCESS) { 
        fprintf(stderr, "%s", status.message);
    }
    
    gru_status_reset(&status);

    return EXIT_FAILURE;
}