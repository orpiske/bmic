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

static void print(const void *obj1, void *d2)
{
    const bmic_object_t *nodeojb = (bmic_object_t *) obj1;

    if (nodeojb == NULL) {
        return;
    }

    printf("Path: %s\n", nodeojb->path);

    switch (nodeojb->type) {
    case STRING:
    {
        printf("%s (string): %s\n", nodeojb->name, nodeojb->data.str);
        break;
    }
    case INTEGER:
    {
        printf("%s (int): %i\n", nodeojb->name, nodeojb->data.number);
        break;
    }
    case BOOLEAN:
    {
        printf("%s (bool): %s\n", nodeojb->name,
               (nodeojb->data.value ? "true" : "false"));
        break;
    }
    case DOUBLE:
    {
        printf("%s (double): %.4f\n", nodeojb->name, nodeojb->data.d);
        break;
    }
    case LIST:
    {
        gru_list_for_each(nodeojb->data.list, print, NULL);
        break;
    }
    case OBJECT:
    {
        if (nodeojb) {
            if (nodeojb->name) {
                printf("Object: %s\n", nodeojb->name);
            }
        }

        break;
    }
    case NULL_TYPE:
    {
        printf("%s (null)\n", nodeojb->name);

        break;
    }

    }
}


static void copy(const void *obj1, void *destptr)
{
    const bmic_object_t *nodeojb = (bmic_object_t *) obj1;
    const bmic_object_t *dest = (bmic_object_t *) destptr;

    if (nodeojb == NULL) {
        return;
    }
    
    gru_status_t status = {0};
    bmic_object_t *cloned = bmic_object_clone(nodeojb, &status);
    bmic_object_add_list_element(dest, cloned);
    
}

bmic_object_t *bmic_object_new(const char *name, gru_status_t *status)
{
    bmic_object_t *ret = NULL;

    if (name == NULL) {
        gru_status_set(status, GRU_FAILURE,
                       "A child object must not have a null name");
    }

    ret = gru_alloc(sizeof (bmic_object_t), status);
    gru_alloc_check(ret, NULL);

    ret->type = NULL_TYPE;
    if (!bmic_object_set_name(ret, name)) {
        gru_status_set(status, GRU_FAILURE, "Unable to set the object name");
        bmic_object_destroy(&ret);
        return NULL;
    }
    
    ret->data.list = NULL;
    ret->data.str = NULL;

    return ret;
}

bmic_object_t *bmic_object_new_root(gru_status_t *status)
{
    bmic_object_t *ret = gru_alloc(sizeof (bmic_object_t), status);
    gru_alloc_check(ret, NULL);

    ret->type = NULL_TYPE;
    if (!bmic_object_set_name(ret, NULL)) {
        gru_status_set(status, GRU_FAILURE, "Unable to set the root object name");

        bmic_object_destroy(&ret);
        return NULL;
    }

    if (!bmic_object_set_path(ret, "")) {
        gru_status_set(status, GRU_FAILURE, "Unable to set the root object path");

        bmic_object_destroy(&ret);
        return NULL;
    }

    ret->self = gru_tree_new(NULL);
    if (!ret) {
        bmic_object_destroy(&ret);

        return NULL;
    }


    return ret;
}

bmic_object_t *bmic_object_clone(const bmic_object_t *other, gru_status_t *status)
{
    bmic_object_t *ret = bmic_object_new_root(status);

    if (!ret) {
        return NULL;
    }

    switch (other->type) {
    case STRING:
    {
        bmic_object_set_string(ret, other->data.str);
        break;
    }
    case INTEGER:
    {
        bmic_object_set_integer(ret, other->data.number);
        break;
    }
    case BOOLEAN:
    {
        bmic_object_set_boolean(ret, other->data.value);
        break;
    }
    case DOUBLE:
    {
        bmic_object_set_double(ret, other->data.d);
        break;
    }
    case NULL_TYPE:
    {
        bmic_object_set_null(ret);
        break;
    }
    case LIST: {
        gru_list_for_each(other->data.list, copy, ret);
    }
    case OBJECT:
    default:
    {
        gru_status_set(status, GRU_FAILURE,
                       "Cloning complex objects is not supported");
        bmic_object_destroy(&ret);
        return NULL;
    }
    }

    return ret;
}

static void bmic_object_destroy_element(const void *nodedata, void *data)
{
    bmic_object_t *obj = (bmic_object_t *) nodedata;

    bmic_object_destroy(&obj);
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

    if (obj->type == LIST) {
        gru_list_for_each(obj->data.list, bmic_object_destroy_element, NULL);

        gru_list_destroy(&obj->data.list);
    }

    if (obj->type == OBJECT) {
        // ROOT element
        if (obj->name == NULL) {
            gru_tree_for_each(obj->self, bmic_object_destroy_element, NULL);

            gru_tree_destroy(&obj->self);
        }
    }

    obj->self = NULL;
    free(obj->name);
    gru_dealloc((void **) ptr);
}

bool bmic_object_set_name(bmic_object_t *obj, const char *name)
{
    int rc = asprintf(&obj->name, "%s", name);
    if (rc == -1) {
        return false;
    }

    return true;
}

bool bmic_object_set_path(bmic_object_t *obj, const char *path, ...)
{
    va_list ap;
    va_start(ap, path);

    int rc = vasprintf(&obj->path, path, ap);
    va_end(ap);

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

void bmic_object_add_list_element(bmic_object_t *parent, bmic_object_t *element)
{
    parent->type = LIST;

    if (!parent->data.list) {
        parent->data.list = gru_list_new(NULL);
        if (!parent->data.list) {
            fprintf(stderr, "Unable to create a new list storage for the element");

            return;
        }
    }

    element->self = gru_tree_add_child(parent->self, element);
    size_t pos = gru_list_count(parent->data.list);
    gru_list_append(parent->data.list, element);

    bmic_object_set_path(element, "%s/%s[%i]", parent->path, element->name, pos);
}

void bmic_object_add_object(bmic_object_t *parent,
                            bmic_object_t *child)
{
    parent->type = OBJECT;

    child->self = gru_tree_add_child(parent->self, child);
    if (!child->self) {
        fprintf(stderr, "Unable to create a new tree storage for the child object");

        return;
    }

    bmic_object_set_path(child, "%s/%s", parent->path, child->name);
}

static bool bmic_compare_name(const void *nodedata, const void *data, void *r)
{
    const bmic_object_t *nodeobj = (bmic_object_t *) nodedata;

    if (nodedata == NULL) {
        if (data == NULL) {
            return true;
        }

        return false;
    }

    if (nodeobj->name != NULL) {
        if (strcmp(nodeobj->name, (const char *) data) == 0) {
            return true;
        }
    }

    return false;

}

const bmic_object_t *bmic_object_find_by_name(const bmic_object_t *parent, const char *name)
{
    const gru_tree_node_t *tn = gru_tree_search(parent->self,
                                                bmic_compare_name, name);

    if (tn) {
        return (const bmic_object_t *) tn->data;
    }


    return NULL;

}

static bool bmic_compare_path(const void *nodedata, const void *data, void *r)
{
    const bmic_object_t *nodeobj = (bmic_object_t *) nodedata;

    if (nodedata == NULL) {
        if (data == NULL) {
            return true;
        }

        return false;
    }

    if (nodeobj->path != NULL) {
        if (strcmp(nodeobj->path, (const char *) data) == 0) {
            return true;
        }
    }

    return false;

}

const bmic_object_t *bmic_object_find_by_path(const bmic_object_t *parent,
                                              const char *path)
{
    const gru_tree_node_t *tn = gru_tree_search(parent->self,
                                                bmic_compare_path, path);

    if (tn) {
        return (const bmic_object_t *) tn->data;
    }


    return NULL;
}

const bmic_object_t *bmic_object_find(const bmic_object_t *parent,
                                      compare_function_t compare,
                                      void *data)
{
    const gru_tree_node_t *tn = gru_tree_search(parent->self,
                                                compare, data);

    if (tn) {
        return (const bmic_object_t *) tn->data;
    }


    return NULL;
}

static bool bmic_object_regex_name(const void *nodedata,
                                   const void *regex, void *r)
{
    const bmic_object_t *nodeobj = (bmic_object_t *) nodedata;

    if (nodedata == NULL) {
        return false;
    }

    // TODO: possibly evaluate using regex instead.

    gru_status_t status = {0};
    bool match = bmic_match(nodeobj->name, (const char *) regex, &status);

    // TODO: improve the error handling here
    if (status.code == GRU_FAILURE) {
        fprintf(stderr, "%s\n", status.message);
    }


    return match;
}

static bool bmic_object_regex_path(const void *nodedata,
                                   const void *regex, void *r)
{
    const bmic_object_t *nodeobj = (bmic_object_t *) nodedata;

    if (nodedata == NULL) {
        return false;
    }

    // TODO: possibly evaluate using regex instead.

    gru_status_t status = {0};
    bool match = bmic_match(nodeobj->path, (const char *) regex, &status);

    // TODO: improve the error handling here
    if (status.code == GRU_FAILURE) {
        fprintf(stderr, "%s\n", status.message);
    }


    return match;
}

const bmic_object_t *bmic_object_find_regex(const bmic_object_t *parent,
                                            const char *regex, int flags)
{
    if (flags & REG_SEARCH_PATH) {
        const gru_tree_node_t *ret = gru_tree_search(parent->self,
                                                     bmic_object_regex_path, regex);

        if (ret != NULL) {
            return (const bmic_object_t *) ret->data;
        }
    }

    if (flags & REG_SEARCH_NAME) {
        const gru_tree_node_t *ret = gru_tree_search(parent->self,
                                                     bmic_object_regex_name, regex);

        if (ret != NULL) {
            return (const bmic_object_t *) ret->data;
        }
    }


    return NULL;
}

/*****
 * DEBUG STUFF 
 */


void bmic_object_print(const bmic_object_t *parent)
{

    gru_tree_for_each(parent->self, print, NULL);

}


