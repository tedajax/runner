#include "game.h"

#include <SDL2/SDL_image.h>

void game_init(Game* self) {
    textures_init("assets");

    textures_load("test.png");
    textures_load("bullet.png");
    textures_load("darkPurple.png");

    self->entityManager = entity_manager_new();
    self->healthSystem = health_system_new(self->entityManager);
    self->spriteSystem = sprite_system_new(self->entityManager);
    self->movementSystem = movement_system_new(self->entityManager);
    self->gravitySystem = gravity_system_new(self->entityManager);
    self->controllerSystem = controller_system_new(self->entityManager);
    self->bulletControllerSystem = bullet_controller_system_new(self->entityManager);
    self->bgManagerSystem = bg_manager_system_new(self->entityManager);

    u32 bgTextureWidth;
    u32 bgTextureHeight;
    SDL_QueryTexture(textures_get("darkPurple.png"), NULL, NULL, &bgTextureWidth, &bgTextureHeight);

    Entity* bgManagerEntity = entity_create_bg_manager(self->entityManager,
        bgTextureWidth,
        bgTextureHeight);

    BgManagerComponent* bgManager = 
        (BgManagerComponent*)entities_get_component(self->entityManager,
        COMPONENT_BG_MANAGER,
        bgManagerEntity);

    for (u32 i = 0; i < bgManager->capacity; ++i) {
        Entity* tile = entity_create_bg_tile(self->entityManager,
            textures_get("darkPurple.png"));

        TransformComponent* tx =
            (TransformComponent*)entities_get_component(self->entityManager,
            COMPONENT_TRANSFORM,
            tile);

        bg_manager_component_add_entity(bgManager, tx);
    }

    self->player = entity_create_player(self->entityManager,
        vec2_init(32.f, 320.f),
        textures_get("test.png"));

    entity_list_init(&self->entities, 64);

    TransformComponent* playerTransform =
        (TransformComponent*)entities_get_component(self->entityManager,
            COMPONENT_TRANSFORM,
            self->player);

    SDL_Rect cameraConstraints = {
        32,
        32,
        400,
        563
    };

    camera_init(&globals.camera, &playerTransform->position, &cameraConstraints);
}

void game_start(Game* self) {
    bg_manager_system_start(self->bgManagerSystem, &self->entities);
}

void game_update(Game* self) {
    health_system_update(self->healthSystem, &self->entities);
    sprite_system_update(self->spriteSystem, &self->entities);
    controller_system_update(self->controllerSystem, &self->entities);
    bullet_controller_system_update(self->bulletControllerSystem, &self->entities);
    gravity_system_update(self->gravitySystem, &self->entities);
    movement_system_update(self->movementSystem, &self->entities);
    bg_manager_system_update(self->bgManagerSystem, &self->entities);

    camera_update(&globals.camera);
}

void game_render(Game* self) {
    health_system_render(self->healthSystem, &self->entities);
    sprite_system_render(self->spriteSystem, &self->entities);
}