#include "component.h"
#include "components.h"

ComponentType COMPONENT_VALUE_TABLE[COMPONENT_LAST];
Hashtable COMPONENT_NAME_TABLE;
bool componentNameTableInitialized = false;

component_deserialize_f COMPONENT_DESERIALIZE_FUNCS[COMPONENT_LAST] = { NULL };

#define COMPONENT_REGISTER(type, datatype) \
    hashtable_insert(&COMPONENT_NAME_TABLE, #type, &COMPONENT_VALUE_TABLE[type]); \
    COMPONENT_DESERIALIZE_FUNCS[type] = COMPONENT_DESERIALIZE_FUNC(type); \
    COMPONENT_SIZE_TABLE[type] = sizeof(datatype);

void component_system_init() {
    if (componentNameTableInitialized) {
        return;
    }

    componentNameTableInitialized = true;

    for (ComponentType t = COMPONENT_INVALID; t < COMPONENT_LAST; ++t) {
        COMPONENT_VALUE_TABLE[t] = t;
    }

    hashtable_init(&COMPONENT_NAME_TABLE, 32, NULL);

    COMPONENT_REGISTER(COMPONENT_TRANSFORM, TransformComponent);
    COMPONENT_REGISTER(COMPONENT_MOVEMENT, MovementComponent);
    COMPONENT_REGISTER(COMPONENT_CONTROLLER, ControllerComponent);
    COMPONENT_REGISTER(COMPONENT_BULLET_CONTROLLER, BulletControllerComponent);
    COMPONENT_REGISTER(COMPONENT_GRAVITY, GravityComponent);
    COMPONENT_REGISTER(COMPONENT_HEALTH, HealthComponent);
    COMPONENT_REGISTER(COMPONENT_SPRITE, SpriteComponent);
    COMPONENT_REGISTER(COMPONENT_BG_MANAGER, BgManagerComponent);
    COMPONENT_REGISTER(COMPONENT_ENEMY, EnemyComponent);
    COMPONENT_REGISTER(COMPONENT_COLLIDER, ColliderComponent);
    COMPONENT_REGISTER(COMPONENT_LUA, LuaComponent);

    ASSERT(COMPONENT_NAME_TABLE.size == COMPONENT_LAST - 1, "Unregistered component type detected.  Did you add a new component recently?");
}

void component_system_terminate() {
    if (!componentNameTableInitialized) {
        return;
    }

    hashtable_free_contents(&COMPONENT_NAME_TABLE);
}

void component_init(Component* self, ComponentType type, u64 size, Entity entity) {
    self->type = type;
    self->size = size;
    self->entity = entity;
}

void component_copy(const Component* source, Component* dest) {
    memcpy(dest, source, (size_t)source->size);
}

void component_set_entity(Component* self, Entity entity) {
    self->entity = entity;
}

void component_free(Component* self) {
    switch (self->type) {
        default:
            break;

        case COMPONENT_BG_MANAGER:
            free(((BgManagerComponent*)self)->transforms);
            break;
    }
}

void component_free_void(void* self) {
    component_free((Component*)self);
}

int component_entity_compare(Component* c1, Component* c2) {
    return c1->entity - c2->entity;
}

ComponentType component_parse_type(char* str) {
    void* pdata = hashtable_get(&COMPONENT_NAME_TABLE, str);
    if (pdata) {
        return *(ComponentType*)pdata;
    }
    return COMPONENT_INVALID;
}

Component* component_deserialize(Config* config, const char* table) {
    char* typeStr = config_get_string(config, table, "component_type");
    ComponentType type = component_parse_type(typeStr);

    ASSERT(type > COMPONENT_INVALID && type < COMPONENT_LAST, "Invalid component deserialization!");

    ASSERT(COMPONENT_DESERIALIZE_FUNCS[type], "No component deserialization function could be found.  Did you register one with this component?");
    
    return COMPONENT_DESERIALIZE_FUNCS[type](config, table);
}

////////////////////////
//// ComponentBatch ////
////////////////////////

void component_batch_init(ComponentBatch* self, u32 capacity) {
    self->components = CALLOC(capacity, Component*);
    self->count = 0;
    self->capacity = capacity;
}

void component_batch_zero(ComponentBatch* self) {
    self->components = NULL;
    self->count = 0;
    self->capacity = 0;
}

void component_batch_add(ComponentBatch* self, Component* component) {
    ASSERT(self->count < self->capacity, "Maximum capacity reached.");
    self->components[self->count] = component;
    ++self->count;
}
