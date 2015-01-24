#include "componentlist.h"

Component* component_list_get_binary_search(ComponentList* self, Entity entity);
i32 component_list_get_binary_search_index(ComponentList* self, Entity entity);

void component_list_init(ComponentList* self, component_compare_f compareFunc) {
    self->compareFunc = compareFunc;
    self->count = 0;
    for (u32 i = 0; i < MAX_ENTITIES; ++i) { self->components[i] = NULL; }
}
void component_list_append(ComponentList* self, Component* component) {
    ASSERT(self->count < MAX_ENTITIES, "Maximum component count reached.");

    self->components[self->count] = component;
    ++self->count;
}

void component_list_insert(ComponentList* self, Component* component) {
    ASSERT(self->count < MAX_ENTITIES, "Maximum component count reached.");

    for (u32 i = 0; i < self->count; ++i) {
        int cmp = self->compareFunc(component, self->components[i]);
        if (cmp == 0) {
            ASSERT(false, "This entity already has this type of component.");
        } else if (cmp < 0) {
            u32 index = (i > 0) ? i - 1 : 0;
            for (u32 j = self->count; j >= index + 1; --j) {
                self->components[j] = self->components[j - 1];
            }
            self->components[index] = component;
            ++self->count;
            return;
        }
    }

    component_list_append(self, component);
}

Component* component_list_get(ComponentList* self, Entity entity) {
    if (self->compareFunc == component_entity_compare) {
        return component_list_get_binary_search(self, entity);
    }

    for (u32 i = 0; i < self->count; ++i) {
        Component* c = self->components[i];
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

    for (i32 i = 0; i < self->count; ++i) {
        Component* c = self->components[i];
        if (c->entity == entity) {
            return i;
        }
    }

    return -1;
}

// Iterative binary search.  Depends on component list using the standard
// entity comparison to sort by.
Component* component_list_get_binary_search(ComponentList* self, Entity entity) {
    i32 iteration = 0;
    i32 mid = self->count / 2;
    while (true) {
        ++iteration;
        Component* c = self->components[mid];
        if (entity == c->entity) {
            return self->components[mid];
        } else {
            i32 div = 2;
            for (u32 i = 0; i < iteration; ++i) { div *= 2; }
            i32 move = self->count / div;
            if (move == 0) { return NULL; }

            if (entity > c->entity) {
                mid += move;
            } else {
                mid -= move;
            }
        }
    }
}

i32 component_list_get_binary_search_index(ComponentList* self, Entity entity) {
    i32 iteration = 0;
    i32 mid = self->count / 2;
    while (true) {
        ++iteration;
        Component* c = self->components[mid];
        if (entity == c->entity) {
            return mid;
        } else {
            i32 div = 2;
            for (u32 i = 0; i < iteration; ++i) { div *= 2; }
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

Component* component_list_get_at(ComponentList* self, u32 index) {
    ASSERT(index < self->count, "Index out of range.");
    return self->components[index];
}

Component* component_list_remove(ComponentList* self, Entity entity) {
    i32 index = component_list_get_index(self, entity);
    ASSERT(index > 0, "Entity could not be found.");
    component_list_remove_at(self, (u32)index);
}

Component* component_list_remove_at(ComponentList* self, u32 index) {
    Component* result = self->components[index];
    
    if (self->compareFunc == NULL) {
        // There's no comparison function so we assume order doesn't matter
        // and therefore we can just swap the component at the index with
        // the one at the back
        self->components[index] = self->components[self->count - 1];
    } else {
        for (u32 i = index; i < self->count - 1; ++i) {
            self->components[i] = self->components[i + 1];
        }
    }

    --self->count;
    return result;
}

void component_list_insertion_sort(ComponentList* self) {
    for (u32 i = 1; 1 < self->count; ++i) {
        u32 j = i;
        Component* c1 = self->components[j - 1];
        Component* c2 = self->components[j];
        while (j > 0 && self->compareFunc(c1, c2) > 0) {
            self->components[j - 1] = c2;
            self->components[j] = c1;
            --j;
        }
    }
}