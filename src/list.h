#ifndef RUNNER_LIST_H
#define RUNNER_LIST_H

#include "memory.h"
#include "types.h"

//double linked list

typedef struct list_node_t {
    void* element;
    struct list_node_t* prev;
    struct list_node_t* next;
} ListNode;

typedef struct list_t {
    ListNode* head;
    ListNode* tail;
    u32 size;
} List;

ListNode* list_node_new(void* element, ListNode* prev, ListNode* next);

List* list_new();
void list_init(List* self);
void list_clear(List* self);
void list_push_back(List* self, void* element);
void list_push_front(List* self, void* element);
void* list_pop_back(List* self);
void* list_pop_front(List* self);
void* list_peek_back(List* self);
void* list_peek_front(List* self);
void* list_index(List* self, u32 index);

#endif