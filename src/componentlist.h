#ifndef RUNNER_COMPONENT_LIST_H
#define RUNNER_COMPONENT_LIST_H

#include "core.h"
#include "component.h"

typedef int(*component_compare_f)(Component*, Component*);

typedef struct component_list_t {
    component_compare_f compareFunc;
    u32 count;
    u32 capacity;
    u32 componentSize;
    Component* components;
    Component* tempStorage;
} ComponentList;

void component_list_init(ComponentList* self, u32 componentSize, component_compare_f compareFunc);
void component_list_set_at(ComponentList* self, u32 index, Component* component);
Component* component_list_get_at(ComponentList* self, u32 index);
void component_list_append(ComponentList* self, Component* component);
Component* component_list_get(ComponentList* self, Entity entity);
i32 component_list_get_index(ComponentList* self, Entity entity);
Component* component_list_remove(ComponentList* self, Entity entity);
Component* component_list_remove_at(ComponentList* self, u32 index);
void component_list_swap(ComponentList* self, Component* c1, Component* c2);

void component_list_insertion_sort(ComponentList* self);

#endif