#ifndef RUNNER_ENTITY_MANAGER_H
#define RUNNER_ENTITY_MANAGER_H

#include "core.h"
#include "vector.h"
#include "dict.h"
#include "entity.h"
#include "component.h"
#include "messaging.h"

// Forward declarations
typedef struct collision_system_t CollisionSystem;
typedef struct aspect_system_t AspectSystem;

typedef struct entity_id_list_t {
    Entity* list;
    u32 size;
    u32 capacity;
} EntityList;

void entity_list_init(EntityList* self, u32 capacity);
void entity_list_resize(EntityList* self, u32 capacity);
void entity_list_free(EntityList* self);

#define ENTITY_MANAGER_MESSAGE_QUEUE_CAPACITY 100
#define ENTITY_MANAGER_MAX_SYSTEM_COUNT 5

typedef struct entity_manager_t {
    Vector entities;
    AspectSystem* systems[COMPONENT_LAST][ENTITY_MANAGER_MAX_SYSTEM_COUNT];
    u32 systemCounts[COMPONENT_LAST];
    Dictionary componentsMap[COMPONENT_LAST];
    u32 lowestEId;
} EntityManager;

EntityManager* entity_manager_new();
void entity_manager_free(EntityManager* self);
void entity_manager_register_system(EntityManager* self, AspectSystem* system);
u32 entities_gen_entity_id(EntityManager* self);
Entity* entities_create_entity(EntityManager* self);
void entities_add_component(EntityManager* self, Component* component, Entity* entity);
Component* entities_get_component(EntityManager* self, ComponentType type, Entity* entity);
DictListNode* entities_get_components(EntityManager* self, ComponentType type, Entity* entity);
bool entities_has_component(EntityManager* self, ComponentType type, Entity* entity);
void entities_remove_entity(EntityManager* self, Entity* entity);
void entities_remove_all_entities(EntityManager* self);
void entities_get_all_of(EntityManager* self, ComponentType type, EntityList* dest);
void entities_send_message(EntityManager* self, Entity* entity, Message message);
u32 entities_entity_count(EntityManager* self);

#define REGISTER_SYSTEM(self, system) \
    entity_manager_register_system(self, (AspectSystem*)system);


#endif