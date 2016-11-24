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

    bmic_debug_print("Path: %s\n", nodeojb->path);

    switch (nodeojb->type) {
    case STRING:
    {
        bmic_debug_print("%s (string): %s\n", nodeojb->name, nodeojb->data.str);
        break;
    }
    case INTEGER:
    {
        bmic_debug_print("%s (int): %i\n", nodeojb->name, nodeojb->data.number);
        break;
    }
    case BOOLEAN:
    {
        bmic_debug_print("%s (bool): %s\n", nodeojb->name,
               (nodeojb->data.value ? "true" : "false"));
        break;
    }
    case DOUBLE:
    {
        bmic_debug_print("%s (double): %.4f\n", nodeojb->name, nodeojb->data.d);
        break;
    }
    case LIST:
    {
        gru_tree_for_each_child(nodeojb->self, print, NULL);
        break;
    }
    case OBJECT:
    {
        if (nodeojb) {
            if (nodeojb->name) {
                bmic_debug_print("Object: %s\n", nodeojb->name);
            }
        }

        break;
    }
    case NULL_TYPE:
    {
        bmic_debug_print("%s (null)\n", nodeojb->name);

        break;
    }
    }
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
    
    ret->data.str = NULL;
    ret->self = NULL;

    return ret;
}

bmic_object_t *bmic_object_new_root(gru_status_t *status)
{
    assert(status != NULL);
    
    bmic_object_t *ret = gru_alloc(sizeof (bmic_object_t), status);
    gru_alloc_check(ret, NULL);

    ret->type = NULL_TYPE;
    ret->name = NULL;

    if (!bmic_object_set_path(ret, "")) {
        gru_status_set(status, GRU_FAILURE, "Unable to set the root object path");

        bmic_object_destroy(&ret);
        return NULL;
    }

    ret->self = gru_tree_new(NULL);
    if (!ret->self) {
        bmic_object_destroy(&ret);

        return NULL;
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
    
    bmic_debug_print("Destroying object %s [%s]\n", obj->name, obj->path);

    if (obj->type == STRING) {
        if (obj->data.str) {
            free(obj->data.str);
        }
    }

    if (obj->type == OBJECT || obj->type == LIST) {
        // ROOT element
        if (obj->name == NULL) {
            gru_tree_for_each(obj->self, bmic_object_destroy_element, NULL);

            gru_tree_destroy(&obj->self);
        }
    }

    obj->self = NULL;
    free(obj->name);
    free(obj->path);
    gru_dealloc((void **) ptr);
}

bool bmic_object_set_name(bmic_object_t *obj, const char *name)
{
    assert(obj != NULL);
    
    int rc = asprintf(&obj->name, "%s", name);
    if (rc == -1) {
        return false;
    }

    return true;
}

bool bmic_object_set_path(bmic_object_t *obj, const char *path, ...)
{
    assert(obj != NULL);
    
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
    assert(obj != NULL && value != NULL);
    
    bmic_debug_print("Setting %s [%s] to %s\n", obj->name, obj->path, value);
    
    obj->type = STRING;
    asprintf(&obj->data.str, "%s", value);
}

void bmic_object_set_integer(bmic_object_t *obj, int32_t value)
{
    assert(obj != NULL);
    
    bmic_debug_print("Setting %s [%s] to %i\n", obj->name, obj->path, value);
    obj->type = INTEGER;
    obj->data.number = value;
}

void bmic_object_set_boolean(bmic_object_t *obj, bool value)
{
    assert(obj != NULL);
    
    
    obj->type = BOOLEAN;
    obj->data.value = value;
}

void bmic_object_set_double(bmic_object_t *obj, double value)
{
    assert(obj != NULL);
    
    obj->type = DOUBLE;
    obj->data.d = value;
}

void bmic_object_set_null(bmic_object_t *obj)
{
    assert(obj != NULL);
    
    obj->type = NULL_TYPE;
    obj->data.str = NULL;
}

void bmic_object_add_list_element(bmic_object_t *parent, bmic_object_t *element)
{
    assert(parent != NULL && element != NULL);
    
    parent->type = LIST;
    uint32_t pos = gru_tree_count_children(parent->self);

    element->self = gru_tree_add_child(parent->self, element);
    if (!element->self) {
        fprintf(stderr, "Unable to create a new tree storage for the child object");

        return;
    }
    
    bmic_object_set_path(element, "%s/%s[%i]", parent->path, element->name, pos);
    
    bmic_debug_print("Adding %s [%s] to %s\n", element->name, element->path, 
                     parent->name);
}

void bmic_object_add_object(bmic_object_t *parent,
                            bmic_object_t *child)
{
    
    assert(parent != NULL && child != NULL);
    
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
    const bmic_object_t *nodeobj = (const bmic_object_t *) nodedata;

    if (nodedata == NULL) {
        if (data == NULL) {
            return true;
        }

        return false;
    }

    if (nodeobj->name != NULL) {
        bmic_debug_print("Comparing %s [%s] %d with %s\n", nodeobj->name, 
                         nodeobj->path, nodeobj->type, (const char *) data);
        if (strcmp(nodeobj->name, (const char *) data) == 0) {
            return true;
        }
    }

    return false;

}

const bmic_object_t *bmic_object_find_by_name(const bmic_object_t *parent, const char *name)
{
    assert(parent != NULL && name != NULL);
    
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
    assert(parent != NULL && path != NULL);
    
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
    assert(parent != NULL);
    
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
    assert(parent != NULL && regex != NULL);
    
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

const bmic_object_t *bmic_object_find_child_by_name(const bmic_object_t *parent, 
                                                    const char *name)
{
    assert(parent != NULL && name != NULL);
    
    const gru_tree_node_t *ret = gru_tree_search_child(parent->self, bmic_compare_name, name);
    if (ret) {
        return ret->data;
    }
    
    return NULL;
}


void bmic_object_for_each(const bmic_object_t *obj, tree_callback_fn callback, 
        void *payload) 
{
    assert(obj != NULL);
    
    gru_tree_for_each(obj->self, callback, payload);
}

void bmic_object_for_each_child(const bmic_object_t *obj, tree_callback_fn callback, 
        void *payload)
{
    assert(obj != NULL);
    
    gru_tree_for_each_child(obj->self, callback, payload);
}

/*****
 * DEBUG STUFF 
 */


void bmic_object_print(const bmic_object_t *parent)
{

    gru_tree_for_each(parent->self, print, NULL);

}


