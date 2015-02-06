#include "componentlist.h"

Component* component_list_get_binary_search(ComponentList* self, Entity entity);
i32 component_list_get_binary_search_index(ComponentList* self, Entity entity);
i32 component_list_binary_search_worker(ComponentList* self, Entity entity, i32 imin, i32 imax);

void component_list_init(ComponentList* self, u32 componentSize, component_compare_f compareFunc) {
    self->compareFunc = compareFunc;
    self->count = 0;
    self->capacity = MAX_ENTITIES;
    self->componentSize = componentSize;
    self->components = (Component*)malloc(self->capacity * componentSize);
    self->tempStorage = (Component*)malloc(componentSize);
}

void component_list_set_at(ComponentList* self, u32 index, Component* component) {
    ASSERT(index < self->capacity, "Index out of range.");
    void* dest = ((u8*)self->components + (index * self->componentSize));
    memcpy(dest, component, self->componentSize);
}

Component* component_list_get_at(ComponentList* self, u32 index) {
    ASSERT(index < self->capacity, "Index out of range.");
    void* source = (u8*)self->components + (index * self->componentSize);
    return (Component*)source;
}

void component_list_append(ComponentList* self, Component* component) {
    ASSERT(self->count < MAX_ENTITIES, "Maximum component count reached.");

    component_list_set_at(self, self->count, component);
    ++self->count;
}

Component* component_list_get(ComponentList* self, Entity entity) {
    if (self->compareFunc == component_entity_compare) {
        return component_list_get_binary_search(self, entity);
    }

    for (u32 i = 0; i < self->count; ++i) {
        Component* c = component_list_get_at(self, i);
        if (c->entity == entity) {
            return c;
        }
    }
    return NULL;
}

i32 component_list_get_index(ComponentList* self, Entity entity) {
    if (self->compareFunc == component_entity_compare) {
        return component_list_get_binary_search_index(self, entity);
    }

    for (u32 i = 0; i < self->count; ++i) {
        Component* c = component_list_get_at(self, i);
        if (c->entity == entity) {
            return (i32)i;
        }
    }

    return -1;
}

// Iterative binary search.  Depends on component list using the standard
// entity comparison to sort by.
Component* component_list_get_binary_search(ComponentList* self, Entity entity) {
    i32 index = component_list_get_binary_search_index(self, entity);
    return (index >= 0) ? component_list_get_at(self, index) : NULL;
}

i32 component_list_get_binary_search_index(ComponentList* self, Entity entity) {
    return component_list_binary_search_worker(self, entity, 0, self->count - 1);
    i32 iteration = 0;
    i32 mid = self->count / 2;
    while (true) {
        ++iteration;
        Component* c = component_list_get_at(self, mid);
        if (entity == c->entity) {
            return mid;
        } else {
            i32 div = 2;
            for (i32 i = 0; i < iteration; ++i) { div *= 2; }
            i32 move = self->count / div;
            if (move == 0) { return -1; }

            if (entity > c->entity) {
                mid += move;
            } else {
                mid -= move;
            }
        }
    }
}

i32 component_list_binary_search_worker(ComponentList* self, Entity entity, i32 imin, i32 imax) {
    while (imin < imax) {
        i32 imid = (imax + imin) / 2;

        if (component_list_get_at(self, imid)->entity < entity) {
            imin = imid + 1;
        } else {
            imax = imid;
        }
    }

    if ((imax == imin) && (component_list_get_at(self, imin)->entity == entity)){
        return imin;
    } else {
        return -1;
    }
}

Component* component_list_remove(ComponentList* self, Entity entity) {
    i32 index = component_list_get_index(self, entity);
    if (index >= 0) {
        return component_list_remove_at(self, (u32)index);
    } else {
        return NULL;
    }
}

Component* component_list_remove_at(ComponentList* self, u32 index) {
    //TODO: This doesn't make sense anymore
    Component* result = component_list_get_at(self, index);
    
    //TODO: Handle edge cases (only 1 element, removing last element, no elements, etc)

    //TODO: investigate if usage patterns make using the swap back method the only thing we need to care about.

    if (self->compareFunc == NULL) {
        // There's no comparison function so we assume order doesn't matter
        // and therefore we can just swap the component at the index with
        // the one at the back
        Component* dest = component_list_get_at(self, index);
        Component* src = component_list_get_at(self, self->count - 1);
        memcpy(dest, src, self->componentSize);
    } else {
        Component* dest = component_list_get_at(self, index);
        Component* src = component_list_get_at(self, index + 1);
        size_t size = (self->count - index) * self->componentSize;
        memcpy(dest, src, size);
    }

    --self->count;
    return result;
}

// Assumes both pointers are in the component list, don't fuck up!
void component_list_swap(ComponentList* self, Component* c1, Component* c2) {
    memcpy(self->tempStorage, c1, self->componentSize);
    memcpy(c1, c2, self->componentSize);
    memcpy(c2, self->tempStorage, self->componentSize);
}

void component_list_insertion_sort(ComponentList* self) {
    for (u32 i = 1; i < self->count; ++i) {
        u32 j = i;
        Component* c1 = component_list_get_at(self, j - 1);
        Component* c2 = component_list_get_at(self, j);
        while (j > 0 && self->compareFunc(c1, c2) > 0) {
            component_list_swap(self, c1, c2);
            --j;
        }
    }
}