#include "controllersystem.h"
#include "entityfactory.h"

void controller_system_init(ControllerSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_CONTROLLER);
}

void controller_system_update(ControllerSystem* self, EntityList* entities) {
    aspect_system_entities((AspectSystem*)self, entities);

    for (u32 i = 0; i < entities->size; ++i) {
        Entity entity = entities->list[i];

        MovementComponent* movement =
            (MovementComponent*)GET_COMPONENT(entity, COMPONENT_MOVEMENT);

        ControllerComponent* controller =
            (ControllerComponent*)GET_COMPONENT(entity, COMPONENT_CONTROLLER);

        TransformComponent* transform =
            (TransformComponent*)GET_COMPONENT(entity, COMPONENT_TRANSFORM);

        REQUIRED_COMPONENTS(transform && movement && controller);

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
			for (u32 i = 0; i < controller->bulletSourceCount; ++i) {
				bullet_source_on(&controller->bulletSources[i]);
			}
        }
        if (input_key_up(SDL_SCANCODE_Z)) {
			for (u32 i = 0; i < controller->bulletSourceCount; ++i) {
				bullet_source_off(&controller->bulletSources[i]);
			}
        }

		for (u32 i = 0; i < controller->bulletSourceCount; ++i) {
			bullet_source_update(&controller->bulletSources[i], globals.time.delta, self->super.entityManager, &transform->position);
		}        

        //if (controller->fireTimer > 0.f) {
        //    controller->fireTimer -= globals.time.delta;
        //}

        //if (input_key(SDL_SCANCODE_Z) && controller->fireTimer <= 0.f) {
        //    Vec2 pos = vec2_clone(&transform->position);
        //    pos.x += 64;
        //    pos.y += 52;
        //    entity_create_bullet(self->super.entityManager,
        //        vec2_clone(&pos),
        //        textures_get("player_bullet_1.png"));
        //    controller->fireTimer = controller->fireDelay;
        //}
    }
}