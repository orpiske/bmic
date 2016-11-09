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
#ifndef BMIC_OBJECT_H
#define BMIC_OBJECT_H

#include <stdbool.h>

#include <common/gru_alloc.h>
#include <common/gru_status.h>

#include <collection/gru_list.h>
#include <collection/gru_tree.h>

#define REG_SEARCH_NAME 1
#define REG_SEARCH_PATH 2


#ifdef __cplusplus
extern "C" {
#endif

typedef enum bmic_value_type_t_ {
    INTEGER,
    STRING,
    BOOLEAN,
    DOUBLE,
    NULL_TYPE,
    LIST,
    OBJECT,
} bmic_value_type_t;

typedef union bmic_value_t_ {
    int32_t number;
    char *str;
    bool value;
    double d;
} bmic_value_t;

typedef struct bmic_object_t_ {
    bmic_value_type_t type;
    char *name;
    char *path;
    
    /**
     * A pointer to their position within the document tree
     */
    gru_tree_node_t *self;
    bmic_value_t data;
} bmic_object_t;

bmic_object_t *bmic_object_new(const char *name, gru_status_t *status);
bmic_object_t *bmic_object_new_root(gru_status_t *status);

void bmic_object_destroy(bmic_object_t **ptr);

bool bmic_object_set_name(bmic_object_t *obj, const char *name);
bool bmic_object_set_path(bmic_object_t *obj, const char *path, ...);

void bmic_object_set_string(bmic_object_t *obj, const char *value);
void bmic_object_set_integer(bmic_object_t *obj, int32_t value);
void bmic_object_set_boolean(bmic_object_t *obj, bool value);
void bmic_object_set_double(bmic_object_t *obj, double value);
void bmic_object_set_null(bmic_object_t *obj);


// Hierarchy manipulation
void bmic_object_add_list_element(bmic_object_t *parent, bmic_object_t *element);
void bmic_object_add_object(bmic_object_t *parent, bmic_object_t *child);

// Path searching

const bmic_object_t *bmic_object_find_by_name(const bmic_object_t *parent, 
        const char *name);
const bmic_object_t *bmic_object_find_by_path(const bmic_object_t *parent, 
                                              const char *path);
const bmic_object_t *bmic_object_find(const bmic_object_t *parent, 
                                compare_function_t compare,
                                              void *data);

const bmic_object_t *bmic_object_find_regex(const bmic_object_t *parent, 
                                const char *regex, int flags);

// Navigates within the object tree
void bmic_object_for_each(const bmic_object_t *obj, tree_callback_fn callback, 
        void *payload);

void bmic_object_for_each_child(const bmic_object_t *obj, tree_callback_fn callback, 
        void *payload);

// DEBUG
void bmic_object_print(const bmic_object_t *parent);


#ifdef __cplusplus
}
#endif

#endif /* BMIC_OBJECT_H */

