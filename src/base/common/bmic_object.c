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
#include "bmic_object.h"

bmic_object_t *bmic_object_new(const char *name, gru_status_t *status)
{
    bmic_object_t *ret = gru_alloc(sizeof (bmic_object_t), status);
    gru_alloc_check(ret, NULL);

    assert(name != NULL);

    ret->type = NULL_TYPE;
    if (!bmic_object_set_name(ret, name)) {
        bmic_object_destroy(&ret);
        return NULL;
    }

    return ret;
}

void bmic_object_destroy(bmic_object_t **ptr)
{
    bmic_object_t *obj = *ptr;

    if (!obj) {
        return;
    }

    if (obj->type == STRING) {
        if (obj->data.str) {
            free(obj->data.str);
        }
    }
    
    if (obj->type == OBJECT) {
        // free all from obj->type.
        // obj->data.object
    }
    
    if (obj->type == LIST) {
        // free all list elements
    }

    free(obj->name);
    gru_dealloc((bmic_object_t **) ptr);
}

bool bmic_object_set_name(bmic_object_t *obj, const char *name) {
    int rc = asprintf(&obj->name, "%s", name);
    if (rc == -1) {
        return false;
    }
    
    return true;
}

void bmic_object_set_string(bmic_object_t *obj, const char *value)
{
    obj->type = STRING;
    asprintf(&obj->data.str, "%s", value);
}

void bmic_object_set_integer(bmic_object_t *obj, int32_t value)
{
    obj->type = INTEGER;
    obj->data.number = value;
}

void bmic_object_set_boolean(bmic_object_t *obj, bool value)
{
    obj->type = BOOLEAN;
    obj->data.value = value;
}

void bmic_object_set_double(bmic_object_t *obj, double value)
{
    obj->type = DOUBLE;
    obj->data.d = value;
}

void bmic_object_set_null(bmic_object_t *obj)
{
    obj->type = NULL_TYPE;
    obj->data.str = NULL;
}

void bmic_object_add_list_element(bmic_object_t *parent, bmic_object_t *element) {
    parent->type = LIST; 
    
    if (!parent->data.list) {
        parent->data.list = gru_list_new(NULL); 
        if (!parent->data.list) {
            fprintf(stderr, "Unable to create a new list storage for the element");

            return;
        }
    }

    gru_list_append(parent->data.list, element);
}

void bmic_object_add_object(bmic_object_t *parent, bmic_object_t *child)
{
    parent->type = OBJECT;

    if (!parent->data.object) {
        parent->data.object = gru_tree_new(child);
        if (!parent->data.object) {
            fprintf(stderr, "Unable to create a new tree storage for the child object");

            return;
        }
    }
    else {
        gru_tree_add_child(parent->data.object, child);
    }
}

static void print(const void *obj1, const void *d2)
{
    bmic_object_t *obj = (bmic_object_t *) obj1;

    printf("Printing: %s\n", (obj && obj->name ? obj->name : "null"));
    bmic_object_print(obj);
}

void bmic_object_print(bmic_object_t *parent)
{
    gru_tree_node_t *tree = parent->data.object;

    switch (parent->type) {
    case STRING:
    {
        printf("String: %s\n", parent->data.str);
        break;
    }
    case INTEGER:
    {
        printf("%s (int): %ld\n", parent->name, parent->data.number);
        break;
    }
    case BOOLEAN:
    {
        printf("Bool: %s\n", (parent->data.value ? "true" : "false"));
        break;
    }
    case LIST:
    {
        printf("############################## List Begin ##############################\n");
        gru_list_for_each(parent->data.list, print, NULL);
        printf("############################## List End ##############################\n");
        
        break;
    }
    case OBJECT:
    {
        printf("---- Object Begin ---\n");
        gru_tree_for_each(parent->data.object, print, NULL);
        printf("---- Object End ----\n");
        break;
    }
    case NULL_TYPE:
    {
        printf("Null\n");

        break;
    }

    }

}