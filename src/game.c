#include "game.h"

#include <SDL2/SDL_image.h>

void game_init(Game* self) {
    textures_init("assets");

    textures_load("test.png");
    textures_load("bullet.png");

    self->entityManager = entity_manager_new();
    self->healthSystem = health_system_new(self->entityManager);
    self->spriteSystem = sprite_system_new(self->entityManager);
    self->movementSystem = movement_system_new(self->entityManager);
    self->gravitySystem = gravity_system_new(self->entityManager);
    self->controllerSystem = controller_system_new(self->entityManager);
    self->bulletControllerSystem = bullet_controller_system_new(self->entityManager);

    self->player = entity_create_player(self->entityManager,
        vec2_init(32.f, 320.f),
        textures_get("test.png"));

    entity_list_init(&self->entities, 64);

    for (u32 i = 0; i < MAX_STARS; ++i) {
        self->stars[i].x = (rand() % 2000) - 1000;
        self->stars[i].y = (rand() % 2000) - 1000;
    }

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

void game_update(Game* self) {
    health_system_update(self->healthSystem, &self->entities);
    sprite_system_update(self->spriteSystem, &self->entities);
    controller_system_update(self->controllerSystem, &self->entities);
    bullet_controller_system_update(self->bulletControllerSystem, &self->entities);
    gravity_system_update(self->gravitySystem, &self->entities);
    movement_system_update(self->movementSystem, &self->entities);

    camera_update(&globals.camera);
}

void game_render(Game* self) {
    for (u32 i = 0; i < MAX_STARS; ++i) {
        SDL_Rect r = {
            (i16)(self->stars[i].x - globals.camera.position.x),
            (i16)(self->stars[i].y - globals.camera.position.y),
            2,
            2
        };
        SDL_RenderCopy(globals.renderer,
            textures_get("bullet.png"),
            NULL,
            &r);
    }

    health_system_render(self->healthSystem, &self->entities);
    sprite_system_render(self->spriteSystem, &self->entities);
}