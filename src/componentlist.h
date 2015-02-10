#ifndef RUNNER_COMPONENT_LIST_H
#define RUNNER_COMPONENT_LIST_H

#include "core.h"
#include "component.h"

typedef int(*component_compare_f)(Component*, Component*);

typedef struct component_list_t {
    component_compare_f compareFunc;
    u32 count;
    u32 capacity;
    Component** components;
} ComponentList;

void component_list_init(ComponentList* self, u32 capacity, component_compare_f compareFunc);
void component_list_free(ComponentList* self);
void component_list_append(ComponentList* self, Component* component);
Component* component_list_get(ComponentList* self, Entity entity);
i32 component_list_get_index(ComponentList* self, Entity entity);
Component* component_list_get_at(ComponentList* self, u32 index);
Component* component_list_remove(ComponentList* self, Entity entity);
Component* component_list_remove_at(ComponentList* self, u32 index);

void component_list_insertion_sort(ComponentList* self);

#endif