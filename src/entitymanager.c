#include "entitymanager.h"

void entity_list_init(EntityList* self, size_t capacity) {
    self->list = (Entity *)calloc(capacity, sizeof(Entity));
    self->capacity = capacity;
    self->size = 0;
}

void entity_list_resize(EntityList* self, size_t capacity) {
    self->capacity = capacity;
    self->list = (Entity *)realloc(self->list, capacity * sizeof(Entity));

    if (capacity < self->size) {
        self->size = capacity;
    }
}

EntityManager* entity_manager_new() {
    EntityManager* self = (EntityManager*)calloc(1, sizeof(EntityManager));

    vector_init(&self->entities, 256, NULL);
    for (u32 i = 0; i < COMPONENT_LAST; ++i) {
        self->componentsMap[i] = NULL;
    }
    self->lowestEId = 1;

    return self;
}

u32 entities_gen_entity_id(EntityManager* self) {
    ASSERT(self->lowestEId < 0xFFFFFFFF, "How the fuck?");
    return self->lowestEId++;
}

Entity* entities_create_entity(EntityManager* self) {
    u32 id = entities_gen_entity_id(self);
    Entity* entity = entity_new(id);
    vector_add(&self->entities, entity);
    return entity;
}

void entities_add_component(EntityManager* self, Component* component, Entity* entity) {
    ASSERT(component->type > COMPONENT_INVALID && component->type < COMPONENT_LAST,
        "Invalid component, did you remember to set the component type in the component constructor?");

    Dictionary* components = self->componentsMap[component->type];

    if (!components) {
        components = dict_new(64);
        self->componentsMap[component->type] = components;
    }
    dict_set(components, entity->id, component);
}

Component* entities_get_component(EntityManager* self, ComponentType type, Entity* entity) {
    return dict_get(self->componentsMap[type], entity->id);
}

void entities_remove_entity(EntityManager* self, Entity* entity) {
    for (u32 t = 0; t < COMPONENT_LAST; ++t) {
        Dictionary* components = self->componentsMap[t];
        Component* component = (Component*)dict_remove(components, entity->id);
        free(component);
    }

    for (u32 i = 0; i < self->entities.size; ++i) {
        Entity* e = (Entity *)vector_index(&self->entities, i);
        if (e->id == entity->id) {
            vector_removeAt(&self->entities, i);
            break;
        }
    }
}

void entities_get_all_of(EntityManager* self, ComponentType type, EntityList* dest) {
    Dictionary* components = self->componentsMap[type];

    dest->size = 0;

    if (components) {
        size_t index = 0;

        for (size_t b = 0; b < components->bucketCount; ++b) {
            DictionaryNode* node = &components->buckets[b];
            while (node != NULL && node->key != DICT_INVALID_KEY) {
                if (node->element != NULL) {
                    if (dest->size >= dest->capacity) {
                        entity_list_resize(dest, dest->capacity * 2);
                    }
                    dest->list[index++].id = node->key;
                    ++dest->size;
                }
                node = node->next;
            }
        }
    }
}