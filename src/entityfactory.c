#include "entityfactory.h"

Entity* entity_create_player(EntityManager* entityManager, Vec2 position, SDL_Texture* texture) {
    Entity* entity = entities_create_entity(entityManager);

    // Transform
    entities_add_component(entityManager,
        (Component*)transform_component_new(position, 0.f, vec2_one()),
        entity);

    entities_add_component(entityManager,
        (Component*)gravity_component_world(),
        entity);

    entities_add_component(entityManager,
        (Component*)movement_component_new(vec2_zero(), 0.f),
        entity);

    entities_add_component(entityManager,
        (Component*)controller_component_new(250.f),
        entity);

    entities_add_component(entityManager,
        (Component*)health_component_new(100),
        entity);

    entities_add_component(entityManager,
        (Component*)sprite_component_new(texture),
        entity);

    return entity;
}