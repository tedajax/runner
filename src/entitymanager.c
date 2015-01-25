#include "entitymanager.h"
#include "collidercomponent.h"
#include "collisionsystem.h"
#include "aspectsystem.h"


//typedef enum component_type_e {
//    COMPONENT_INVALID,
//
//    COMPONENT_TRANSFORM,
//    COMPONENT_MOVEMENT,
//
//    COMPONENT_CONTROLLER,
//    COMPONENT_BULLET_CONTROLLER,
//    COMPONENT_GRAVITY,
//
//    COMPONENT_HEALTH,
//    COMPONENT_SPRITE,
//
//    COMPONENT_BG_MANAGER,
//
//    COMPONENT_ENEMY,
//
//    COMPONENT_COLLIDER,
//
//    COMPONENT_LAST,
//} ComponentType;

component_compare_f componentCompareFunctions[COMPONENT_LAST] = {
    NULL,
    component_entity_compare,
    component_entity_compare,
    component_entity_compare,
    component_entity_compare,
    component_entity_compare,
    component_entity_compare,
    component_entity_compare,
    component_entity_compare,
    component_entity_compare,
    collider_component_compare
};

void entities_internal_remove_entity(EntityManager* self, Entity entity);
void entities_internal_send_message(EntityManager* self, TargetedMessage message);

POOL_IMPLEMENTATION(Entity);

void entity_queue_init(EntityQueue* self) {
    self->length = 0;
    self->head = 0;
    self->tail = 0;
}

void entity_queue_push(EntityQueue* self, Entity entity) {
    ASSERT(self->length < MAX_ENTITIES, "Reached maximum size of entity queue.");
    self->entities[self->tail] = entity;
    ++self->tail;
    ++self->length;
}

Entity entity_queue_pop(EntityQueue* self) {
    Entity result = self->entities[self->head];
    ++self->head;
    if (self->head == self->tail) {
        self->head = 0;
        self->tail = 0;
    }
    --self->length;
    return result;
}

void entity_list_init(EntityList* self, u32 capacity) {
    self->list = (Entity *)calloc(capacity, sizeof(Entity));
    self->capacity = capacity;
    self->size = 0;
}

void entity_list_resize(EntityList* self, u32 capacity) {
    self->capacity = capacity;
    self->list = (Entity *)realloc(self->list, capacity * sizeof(Entity));

    if (capacity < self->size) {
        self->size = capacity;
    }
}

void entity_list_free(EntityList* self) {
    free(self->list);
}

EntityManager* entity_manager_new() {
    EntityManager* self = (EntityManager*)calloc(1, sizeof(EntityManager));

    POOL_INIT(Entity)(&self->entities, MAX_ENTITIES, 0);
    for (u32 i = 0; i < COMPONENT_LAST; ++i) {
        component_list_init(&self->componentsMap[i], componentCompareFunctions[i]);
    }
    self->lowestEId = 1;

    for (u32 c = 0; c < COMPONENT_LAST; ++c) {
        for (u32 i = 0; i < ENTITY_MANAGER_MAX_SYSTEM_COUNT; ++i) {
            self->systems[c][i] = NULL;
        }
        self->systemCounts[c] = 0;
    }

    entity_queue_init(&self->removeQueue);
    message_event_queue_init(&self->eventQueue);

    return self;
}

void entity_manager_free(EntityManager* self) {
    entities_remove_all_entities(self);
    for (u32 i = 0; i < COMPONENT_LAST; ++i) {
        //dict_clear(&self->componentsMap[i]);
        // Todo component_list_free
    }
    POOL_FREE(Entity)(&self->entities);
    free(self->entities.data);
    free(self);
}

void entity_manager_register_system(EntityManager* self, AspectSystem* system) {
    ASSERT(self && system, "");
    
    ComponentType type = system->systemType;

    ASSERT(self->systemCounts[type] < ENTITY_MANAGER_MAX_SYSTEM_COUNT - 1, "");

    u32 count = self->systemCounts[type];
    self->systems[type][count] = system;
    ++self->systemCounts[type];
}

i32 entities_gen_entity_id(EntityManager* self) {
    ASSERT(self->lowestEId < 0xFFFFFFFF, "How the fuck?");
    return self->lowestEId++;
}

Entity entities_create_entity(EntityManager* self) {
    Entity entity = entities_gen_entity_id(self);
    POOL_INSERT(Entity)(&self->entities, (entity));
    return entity;
}

void entities_add_component(EntityManager* self, Component* component, Entity entity) {
    ASSERT(component->type > COMPONENT_INVALID && component->type < COMPONENT_LAST,
        "Invalid component, did you remember to set the component type in the component constructor?");

    component_list_insert(&self->componentsMap[component->type], component);
}

Component* entities_get_component(EntityManager* self, ComponentType type, Entity entity) {
    return component_list_get(&self->componentsMap[type], entity);
}

ComponentList* entities_get_all_components(EntityManager* self, ComponentType type) {
    return &self->componentsMap[type];
}

bool entities_has_component(EntityManager* self, ComponentType type, Entity entity) {
    return entities_get_component(self, type, entity) != NULL;
}

void entities_internal_remove_entity(EntityManager* self, Entity entity) {
    Message msg;
    msg.type = MESSAGE_ENTITY_REMOVED;

    for (u32 t = COMPONENT_INVALID + 1; t < COMPONENT_LAST; ++t) {
        for (u32 i = 0; i < self->systemCounts[t]; ++i) {
            aspect_system_send_message((AspectSystem*)self->systems[t][i],
                entity,
                msg);
        }

        Component* toRemove = component_list_remove(&self->componentsMap[t], entity);
        if (toRemove != NULL) {
            component_free_void(toRemove);
        }
    }

    for (u32 i = 0; i < self->entities.capacity; ++i) {
        Entity* e = POOL_GET(Entity)(&self->entities, i);
        if (e && *e == entity) {
            POOL_REMOVE_AT(Entity)(&self->entities, i);
            break;
        }
    }
}

void entities_remove_entity(EntityManager* self, Entity entity) {
    entity_queue_push(&self->removeQueue, entity);
}

void entities_remove_all_entities(EntityManager* self) {
    u32 len = self->entities.capacity;
    for (u32 i = 0; i < len; ++i) {
        Entity e = *POOL_GET(Entity)(&self->entities, i);
        if (e) {
            entities_remove_entity(self, e);
        }
    }
}

void entities_get_all_of(EntityManager* self, ComponentType type, EntityList* dest) {
    /* 
        This function is no longer necessary with the new architecture but it's worth
        recreating while we try and get everything running with new stuff.
    */

    ComponentList* components = &self->componentsMap[type];
    dest->size = 0;
    for (u32 i = 0; i < components->count; ++i) {
        ASSERT(dest->size < dest->capacity, "Reached maximum size of entity list.  Make it bigger!");
        dest->list[i] = components->components[i]->entity;
        ++dest->size;
    }

    ///*
    //  Right now this function is doing a LOT of work every frame.
    //  This needs to be reduced because every system that gets added
    //  increases the call count to here by 2 (1 update, 1 render)
    //  
    //  Strategies for reducing cost...
    //  
    //  * Keep dictionary compacted so we don't have to traverse the whole thing.
    //    * Increases cost of dict_remove (have to rearrange bucket when an empty spot opens up)
    //  * Get rid of linked list of elements on a node, make it a static array.
    //    * Less flexible but really should be done.  Would reduce branching here.
    //  * Not really an issue but the resize step should be removed and the entity list should just be sized properly.
    //*/

    //Dictionary* components = &self->componentsMap[type];
    //dest->size = 0;
    //u32 index = 0;
    //for (u32 b = 0; b < DICT_BUCKET_COUNT; ++b) {
    //    for (u32 i = 0; i < DICT_MAX_ELEMENTS_PER_BUCKET; ++i) {
    //        DictionaryNode* node = &components->buckets[b][i];
    //        
    //        // This branch kills the performance
    //        if (node->list != NULL) {
    //            // So does this one
    //            ASSERT(dest->size < dest->capacity, "Reached maximum size of entity list.  Make it bigger!");
    //            dest->list[index++] = node->key;
    //            ++dest->size;
    //        }
    //    }
    //}
}

void entities_internal_send_message(EntityManager* self, TargetedMessage message) {
    for (u32 type = COMPONENT_INVALID + 1; type < COMPONENT_LAST; ++type) {
        if (!entities_has_component(self, type, message.target)) {
            continue;
        }

        for (u32 i = 0; i < self->systemCounts[type]; ++i) {
            aspect_system_send_message((AspectSystem*)self->systems[type][i],
                message.target,
                message.message);
        }
    }
}

void entities_send_message(EntityManager* self, Entity entity, Message message) {
    message_event_queue_push(&self->eventQueue, entity, message);
}

void entities_send_message_deferred(EntityManager* self, Entity entity, Message message) {
    message_event_queue_push_deferred(&self->eventQueue, entity, message);
}

void entities_update(EntityManager* self) {
    const i32 maxMessages = 0xFFFFFF;
    i32 messageCount = 0;
    message_event_queue_processing_lock(&self->eventQueue);
    while (message_event_queue_size(&self->eventQueue) > 0 && messageCount < maxMessages) {
        TargetedMessage message = message_event_queue_pop(&self->eventQueue);
        entities_internal_send_message(self, message);
        ++messageCount;
    }
    message_event_queue_processing_unlock(&self->eventQueue);

    while (self->removeQueue.length > 0) {
        entities_internal_remove_entity(self, entity_queue_pop(&self->removeQueue));
    }
}