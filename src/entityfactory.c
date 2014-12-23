#include "entityfactory.h"

Entity* entity_create_player(EntityManager* entityManager, Vec2 position, SDL_Texture* texture) {
    Entity* entity = entities_create_entity(entityManager);

    TransformComponent* transform = transform_component_new(position,
        0.f,
        vec2_one());

    // Transform
    entities_add_component(entityManager,
        (Component*)transform,
        entity);

    // entities_add_component(entityManager,
    //     (Component*)gravity_component_world(),
    //     entity);

    MovementComponent* movement = movement_component_new(vec2_zero(), 0.f);
    movement->constrainToCamera = true;

    entities_add_component(entityManager,
        (Component*)movement,
        entity);

    entities_add_component(entityManager,
        (Component*)controller_component_new("player.ini", "player"),
        entity);

    entities_add_component(entityManager,
        (Component*)health_component_new(100000),
        entity);

    entities_add_component(entityManager,
        (Component*)sprite_component_new(texture, 1),
        entity);

    Collider collider;
    collider_init_aabb(&collider,
        COLLIDER_LAYER_PLAYER,
        transform,
        vec2_init(37.5f, 56.f),
        75.f,
        112.f);

    entities_add_component(entityManager,
        (Component*)collider_component_new(&collider),
        entity);

    return entity;
}

Entity* entity_create_bullet(EntityManager* entityManager, BulletConfig* config, Vec2 position, SDL_Texture* texture) {
    Entity* entity = entities_create_entity(entityManager);

    TransformComponent* transform = transform_component_new(position, 0.f, vec2_one());

    entities_add_component(entityManager,
        (Component*)transform,
        entity);

    entities_add_component(entityManager,
        (Component*)movement_component_new(vec2_zero(), 0.f),
        entity);

    entities_add_component(entityManager,
        (Component*)bullet_controller_component_new(config),
        entity);

    entities_add_component(entityManager,
        (Component*)sprite_component_new(texture, 1),
        entity);

    Collider collider;
    collider_init_obb(&collider,
        COLLIDER_LAYER_PLAYER_PROJECTILE,
        transform,
        vec2_init(4, 4),
        8.f,
        8.f,
        0.f);

    entities_add_component(entityManager,
        (Component*)collider_component_new(&collider),
        entity);

    return entity;
}

Entity* entity_create_bg_tile(EntityManager* entityManager, SDL_Texture* texture) {
    Entity* entity = entities_create_entity(entityManager);

    entities_add_component(entityManager,
        (Component*)transform_component_new(vec2_zero(), 0.f, vec2_one()),
        entity);

    entities_add_component(entityManager,
        (Component*)sprite_component_new(texture, 0),
        entity);

    return entity;
}

Entity* entity_create_bg_manager(EntityManager* entityManager, u32 twidth, u32 theight) {
    Entity* entity = entities_create_entity(entityManager);

    entities_add_component(entityManager,
        (Component*)bg_manager_component_new(twidth, theight),
        entity);

    return entity;
}

Entity* entity_create_basic_enemy(EntityManager* entityManager, Vec2 position) {
    Entity* entity = entities_create_entity(entityManager);

    TransformComponent* transform = transform_component_new(position,
        0.f,
        vec2_one());

    entities_add_component(entityManager,
        (Component*)transform,
        entity);

    entities_add_component(entityManager,
        (Component*)movement_component_new(vec2_zero(), 0.f),
        entity);

    entities_add_component(entityManager,
        (Component*)enemy_component_new(ENEMY_TYPE_BASIC),
        entity);

    entities_add_component(entityManager,
        (Component*)health_component_new(100),
        entity);

    entities_add_component(entityManager,
        (Component*)sprite_component_new(textures_get("enemy_red_1.png"), 1),
        entity);

    Collider collider;
    collider_init_aabb(&collider,
        COLLIDER_LAYER_ENEMY,
        transform,
        vec2_init(42.f, 51.5f),
        84.f,
        103.f);

    entities_add_component(entityManager,
        (Component*)collider_component_new(&collider),
        entity);

    return entity;
}