#include "game.h"

#include <SDL2/SDL_image.h>

void game_init(Game* self) {
    self->entityManager = entity_manager_new();
    self->healthSystem = health_system_new(self->entityManager);
    self->spriteSystem = sprite_system_new(self->entityManager);
    self->movementSystem = movement_system_new(self->entityManager);
    self->controllerSystem = controller_system_new(self->entityManager);

    self->player = entities_create_entity(self->entityManager);

    entities_add_component(self->entityManager,
        (Component *)transform_component_new(vec2_zero(), 0.f, vec2_one()),
        self->player);

    entities_add_component(self->entityManager,
        (Component *)movement_component_new(vec2_zero(), 0.f),
        self->player);

    entities_add_component(self->entityManager,
        (Component *)controller_component_new(10.f),
        self->player);

    entities_add_component(self->entityManager,
        (Component *)health_component_new(100),
        self->player);

    SDL_Surface* surface = IMG_Load("assets/test.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(globals.renderer,
        surface);

    //SDL_FreeSurface(surface);

    entities_add_component(self->entityManager,
        (Component *)sprite_component_new(texture),
        self->player);
}

void game_update(Game* self) {
    health_system_update(self->healthSystem);
    sprite_system_update(self->spriteSystem);
    controller_system_update(self->controllerSystem);
    movement_system_update(self->movementSystem);
}

void game_render(Game* self) {
    health_system_render(self->healthSystem);
    sprite_system_render(self->spriteSystem);
}