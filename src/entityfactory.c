#include "entityfactory.h"

Entity entity_create_player(EntityManager* entityManager, Vec2 position, Atlas* atlas, char* spriteName) {
    Entity entity = entities_create_entity(entityManager);

    // Transform
    entities_add_component(entityManager,
        (Component*)transform_component_new(entity, position, 0.f, vec2_one()));

    // entities_add_component(entityManager,
    //     (Component*)gravity_component_world(),
    //     entity);

    MovementComponent* movement = movement_component_new(entity, vec2_zero(), 0.f);
    movement->constrainToCamera = true;

    entities_add_component(entityManager,
        (Component*)movement);

    entities_add_component(entityManager,
        (Component*)controller_component_new(entity, config_get("player.ini"), "player"));

    entities_add_component(entityManager,
        (Component*)health_component_new(entity, 100000));

    entities_add_component(entityManager,
        (Component*)sprite_component_new(entity, atlas, spriteName, 1));

    Collider collider;
    collider_init_aabb(&collider,
        entity,
        COLLIDER_LAYER_PLAYER,
        vec2_init(37.5f, 56.f),
        75.f,
        112.f);

    entities_add_component(entityManager,
        (Component*)collider_component_new(entity, &collider));

    entities_add_component(entityManager,
        (Component*)lua_component_new(entity, "assets/scripts/test_component.lua"));

    return entity;
}

Entity entity_create_bullet(EntityManager* entityManager, BulletConfig* bulletConfig, ColliderConfig* colliderConfig, Vec2 position, Atlas* atlas, char* spriteName) {
    Entity entity = entities_create_entity(entityManager);

    entities_add_component(entityManager,
        (Component*)transform_component_new(entity, position, 0.f, vec2_one()));

    entities_add_component(entityManager,
        (Component*)movement_component_new(entity, vec2_zero(), 0.f));

    entities_add_component(entityManager,
        (Component*)bullet_controller_component_new(entity, bulletConfig));

    entities_add_component(entityManager,
        (Component*)sprite_component_new(entity, atlas, spriteName, 1));

    Collider collider;
    collider_init_config(&collider, entity, colliderConfig);

    entities_add_component(entityManager,
        (Component*)collider_component_new(entity, &collider));

    return entity;
}

Entity entity_create_bg_tile(EntityManager* entityManager, Atlas* atlas, char* spriteName) {
    Entity entity = entities_create_entity(entityManager);

    entities_add_component(entityManager,
        (Component*)transform_component_new(entity, vec2_zero(), 0.f, vec2_one()));

    entities_add_component(entityManager,
        (Component*)sprite_component_new(entity, atlas, spriteName, -10));

    return entity;
}

Entity entity_create_bg_manager(EntityManager* entityManager, u32 twidth, u32 theight) {
    Entity entity = entities_create_entity(entityManager);

    entities_add_component(entityManager,
        (Component*)bg_manager_component_new(entity, twidth, theight));

    return entity;
}

Entity entity_create_basic_enemy(EntityManager* entityManager, Vec2 position) {
    Entity entity = entities_create_entity(entityManager);

    entities_add_component(entityManager,
        (Component*)transform_component_new(entity, position, 0.f, vec2_one()));

    entities_add_component(entityManager,
        (Component*)movement_component_new(entity, vec2_zero(), 0.f));

    entities_add_component(entityManager,
        (Component*)enemy_component_new(entity, ENEMY_TYPE_BASIC));

    entities_add_component(entityManager,
        (Component*)health_component_new(entity, 100));

    entities_add_component(entityManager,
        (Component*)sprite_component_new(entity, atlas_get("atlas1"), "enemy_red_1", 1));

    Collider collider;
    collider_init_aabb(&collider,
        entity,
        COLLIDER_LAYER_ENEMY,
        vec2_init(42.f, 51.5f),
        84.f,
        103.f);

    entities_add_component(entityManager,
        (Component*)collider_component_new(entity, &collider));

    return entity;
}