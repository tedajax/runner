#include "list.h"
#include "debug.h"

ListNode* list_node_new(void* element, ListNode* prev, ListNode* next) {
    ListNode* self = (ListNode*)calloc(1, sizeof(ListNode));

    self->element = element;
    self->prev = prev;
    self->next = next;

    return self;
}

List* list_new() {
    List* self = (List*)calloc(1, sizeof(List));

    self->head = NULL;
    self->tail = NULL;
    self->size = 0;

    return self;
}

void list_init(List* self) {
    self->head = NULL;
    self->tail = NULL;
    self->size = 0;
}

void list_clear(List* self) {
    ListNode* slider = self->head;

    while (slider) {
        ListNode* next = slider->next;
        free(slider);
        slider = next;
    }

    self->head = NULL;
    self->tail = NULL;
    self->size = 0;
}

void list_push_back(List* self, void* element) {
    ListNode* newNode = list_node_new(element, NULL, NULL);

    if (self->size == 0) {
        self->head = newNode;
        self->tail = newNode;
    } else {
        newNode->prev = self->tail;
        self->tail->next = newNode;
        self->tail = newNode;
    }

    ++self->size;
}

void list_push_front(List* self, void* element) {
    ListNode* newNode = list_node_new(element, NULL, NULL);

    if (self->size == 0) {
        self->head = newNode;
        self->tail = newNode;
    } else {
        newNode->next = self->head;
        self->head->prev = newNode;
        self->head = newNode;
    }

    ++self->size;
}

void* list_pop_back(List* self) {
    if (self->size == 0) {
        return NULL;
    }

    ListNode* back = self->tail;
    void* element = back->element;

    self->tail = self->tail->prev;

    if (self->tail) {
        self->tail->next = NULL;
    }

    --self->size;

    free(back);
    return element;
}

void* list_pop_front(List* self) {
    if (self->size == 0) {
        return NULL;
    }

    ListNode* front = self->head;
    void* element = front->element;

    self->head = front->next;

    if (self->head) {
        self->head->prev = NULL;
    }

    --self->size;

    free(front);
    return element;
}

void* list_peek_back(List* self) {
    if (self->size == 0) {
        return NULL;
    } else {
        return self->tail->element;
    }
}

void* list_peek_front(List* self) {
    if (self->size == 0) {
        return NULL;
    } else {
        return self->head->element;
    }
}

void* list_index(List* self, size_t index) {
    ASSERT(index >= 0 && index < self->size, "Index out of bounds.");

    ListNode* slider = self->head;

    for (size_t i = 0; i < index; ++i) {
        slider = slider->next;
    }

    return slider->element;
}