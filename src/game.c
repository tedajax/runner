#include "game.h"

#include <SDL2/SDL_image.h>

#include "particles.h"
#include "config.h"

ParticleEmitter testEmitter;

void game_init(Game* self) {  
    textures_init("assets");

    textures_load("player_ship.png");
    textures_load("player_bullet_1.png");
    textures_load("bg_dark_purple.png");
    textures_load("enemy_red_1.png");
    textures_load("fireparticle.png");

    self->entityManager = entity_manager_new();
    health_system_init(&self->healthSystem, self->entityManager);
    sprite_system_init(&self->spriteSystem, self->entityManager, 1);
    sprite_system_init(&self->backgroundSpriteSystem, self->entityManager, 0);
    movement_system_init(&self->movementSystem, self->entityManager);
    gravity_system_init(&self->gravitySystem, self->entityManager);
    controller_system_init(&self->controllerSystem, self->entityManager);
    bullet_controller_system_init(&self->bulletControllerSystem, self->entityManager);
    bg_manager_system_init(&self->bgManagerSystem, self->entityManager);
    enemy_system_init(&self->enemySystem, self->entityManager);
    collision_system_init(&self->collisionSystem, self->entityManager);

    int bgTextureWidth;
    int bgTextureHeight;
    SDL_QueryTexture(textures_get("bg_dark_purple.png"), NULL, NULL, &bgTextureWidth, &bgTextureHeight);

    Entity* bgManagerEntity = entity_create_bg_manager(self->entityManager,
        (u32)bgTextureWidth,
        (u32)bgTextureHeight);

    BgManagerComponent* bgManager = 
        (BgManagerComponent*)entities_get_component(self->entityManager,
        COMPONENT_BG_MANAGER,
        bgManagerEntity);

    for (u32 i = 0; i < bgManager->capacity; ++i) {
        Entity* tile = entity_create_bg_tile(self->entityManager,
            textures_get("bg_dark_purple.png"));

        TransformComponent* tx =
            (TransformComponent*)entities_get_component(self->entityManager,
            COMPONENT_TRANSFORM,
            tile);

        bg_manager_component_add_entity(bgManager, tx);
    }

    self->player = entity_create_player(self->entityManager,
        vec2_init(32.f, 320.f),
        textures_get("player_ship.png"));

    globals.player = self->player;

    entity_list_init(&self->entities, 512);

    //TransformComponent* playerTransform =
    //    (TransformComponent*)entities_get_component(self->entityManager,
    //        COMPONENT_TRANSFORM,
    //        self->player);

    SDL_Rect cameraConstraints = {
        32,
        32,
        400,
        563
    };

    for (u32 i = 0; i < 50; ++i) {
        entity_create_basic_enemy(self->entityManager, vec2_init(i * 250.f, randf((f32)globals.world.height - 100.f)));
    }

    camera_init(&globals.camera, NULL, &cameraConstraints);


    emitter_config(&testEmitter, "particles.ini", "fire");
    //emitter_init(&testEmitter, 1000, textures_get("fireparticle.png"));
}

void game_quit(Game* self) {
    entity_list_free(&self->entities);
    entity_manager_free(self->entityManager);
}

void game_start(Game* self) {
    bg_manager_system_start(&self->bgManagerSystem, &self->entities);
    collision_system_start(&self->collisionSystem, &self->entities);
}

void game_update(Game* self) {
    health_system_update(&self->healthSystem, &self->entities);
    sprite_system_update(&self->backgroundSpriteSystem, &self->entities);
    sprite_system_update(&self->spriteSystem, &self->entities);
    controller_system_update(&self->controllerSystem, &self->entities);
    bullet_controller_system_update(&self->bulletControllerSystem, &self->entities);
    gravity_system_update(&self->gravitySystem, &self->entities);
    enemy_system_update(&self->enemySystem, &self->entities);
    movement_system_update(&self->movementSystem, &self->entities);
    bg_manager_system_update(&self->bgManagerSystem, &self->entities);
    collision_system_update(&self->collisionSystem, &self->entities);

    if (input_key_down(SDL_SCANCODE_E)) {
        printf("Entities: %u\n", entities_entity_count(self->entityManager));
    }

    emitter_update(&testEmitter);

    camera_update(&globals.camera);
}

void game_render(Game* self) {
    sprite_system_render(&self->backgroundSpriteSystem, &self->entities);
    sprite_system_render(&self->spriteSystem, &self->entities);
    collision_system_render(&self->collisionSystem, &self->entities);

    Vec2 position;
    position.x = 500;
    position.y = 300;
    emitter_render(&testEmitter, &position);
}