#include "game.h"

void game_init(Game* self) {
    self->entityManager = entity_manager_new();
    self->healthSystem = health_system_new(self->entityManager);
    self->player = entities_create_entity(self->entityManager);

    entities_add_component(self->entityManager,
        (Component *)health_component_new(100),
        self->player);
}

void game_update(Game* self) {
    health_system_update(self->healthSystem);
}

void game_render(Game* self) {
    health_system_render(self->healthSystem);
}