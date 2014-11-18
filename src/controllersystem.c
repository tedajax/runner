#include "controllersystem.h"
#include "entityfactory.h"

ControllerSystem* controller_system_new(EntityManager* entityManager) {
    ControllerSystem* self = (ControllerSystem*)calloc(1, sizeof(ControllerSystem));

    aspect_system_init(&self->super, entityManager, COMPONENT_CONTROLLER);

    return self;
}

void controller_system_update(ControllerSystem* self) {
    EntityList entities;
    entity_list_init(&entities, 64);
    aspect_system_entities((AspectSystem*)self, &entities);

    for (u32 i = 0; i < entities.size; ++i) {
        Entity entity = entities.list[i];

        MovementComponent* movement = GET_COMPONENT(MovementComponent, COMPONENT_MOVEMENT, entity);

        ControllerComponent* controller = GET_COMPONENT(ControllerComponent, COMPONENT_CONTROLLER, entity);

        if (!movement || !controller) {
            continue;
        }

        f32 x = 0;
        f32 y = 0;

        if (input_key(SDL_SCANCODE_LEFT)) {
            x -= 1.f;
        }

        if (input_key(SDL_SCANCODE_RIGHT)) {
            x += 1.f;
        }

        if (input_key(SDL_SCANCODE_UP)) {
            y -= 1.f;
        }

        if (input_key(SDL_SCANCODE_DOWN)) {
            y += 1.f;
        }

        //printf("%f\n", controller->moveSpeed);

        x *= controller->moveSpeed;
        y *= controller->moveSpeed;

        vec2_set(&movement->velocity, x, y);

        if (input_key_down(SDL_SCANCODE_Z)) {
            entity_create_bullet(self->super.entityManager,
                vec2_init(100.f, 200.f),
                globals.bulletTexture);
        }
    }

    free(entities.list);
}