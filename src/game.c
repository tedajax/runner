#include "game.h"

#include <SDL2/SDL_image.h>

void game_init(Game* self) {
    self->entityManager = entity_manager_new();
    self->healthSystem = health_system_new(self->entityManager);
    self->spriteSystem = sprite_system_new(self->entityManager);
    self->movementSystem = movement_system_new(self->entityManager);
    self->controllerSystem = controller_system_new(self->entityManager);

    SDL_Surface* surface = IMG_Load("assets/test.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(globals.renderer,
        surface);

    self->player = entity_create_player(self->entityManager, vec2_zero(), texture);
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