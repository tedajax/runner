#include "spritesystem.h"

void sprite_system_init(SpriteSystem* self, EntityManager* entityManager, u32 layer) {
    aspect_system_init(&self->super, entityManager, COMPONENT_SPRITE);
    self->layer = layer;

    REGISTER_SYSTEM_HANDLER(MESSAGE_DAMAGE, sprite_system_on_damage);
}

void sprite_system_update(SpriteSystem* self, EntityList* entities) {
    aspect_system_entities((AspectSystem*)self, entities);

    for (u32 i = 0; i < entities->size; ++i) {
        Entity entity = entities->list[i];

        TransformComponent* transform = (TransformComponent*)entities_get_component(
            self->super.entityManager,
            COMPONENT_TRANSFORM,
            &entity);

        SpriteComponent* sprite = (SpriteComponent*)entities_get_component(
            self->super.entityManager,
            COMPONENT_SPRITE,
            &entity);

        REQUIRED_COMPONENTS(transform && sprite);

        if (sprite->layer != self->layer) {
            continue;
        }

        if (sprite->redTimer > 0.f) {
            sprite->redTimer -= globals.time.delta;
        }
    }
}

void sprite_system_render(SpriteSystem* self, EntityList* entities) {
    aspect_system_entities((AspectSystem*)self, entities);

    for (u32 i = 0; i < entities->size; ++i) {
        Entity entity = entities->list[i];

        TransformComponent* transform = (TransformComponent*)entities_get_component(
            self->super.entityManager,
            COMPONENT_TRANSFORM,
            &entity);

        SpriteComponent* sprite = (SpriteComponent*)entities_get_component(
            self->super.entityManager,
            COMPONENT_SPRITE,
            &entity);

        ASSERT(transform && sprite, "Missing required component for sprite system.");

        if (sprite->layer != self->layer) {
            continue;
        }

        if (sprite->texture) {
            SDL_Rect dest;
            sprite_component_destination(sprite, transform, &dest);

            Rect spriteRect;
            rect_copy_sdl_rect(&dest, &spriteRect);
            
            if (!rect_intersects(&globals.camera.cameraView, &spriteRect)) {
                continue;
            }

            if (sprite->redTimer > 0.f) {
                SDL_SetTextureColorMod(sprite->texture, 255, 0, 0);
            }

            SDL_RenderCopyEx(globals.renderer,
                sprite->texture,
                NULL, //source
                &dest, //destination
                transform->rotation,
                NULL,
                SDL_FLIP_NONE);

            if (sprite->redTimer > 0.f ) {
                SDL_SetTextureColorMod(sprite->texture, 255, 255, 255);
            }            
        }
    }
}

void sprite_system_on_damage(AspectSystem* system, Entity* entity, Message message) {
    SpriteSystem* self = (SpriteSystem*)system;

    SpriteComponent* sprite =
        (SpriteComponent*)GET_COMPONENT(*entity, COMPONENT_SPRITE);

    REQUIRED_COMPONENTS(sprite);

    sprite->redTimer = 0.05f;
}