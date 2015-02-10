#include "spritesystem.h"

void sprite_system_init(SpriteSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_SPRITE, MAX_ENTITIES);
    SET_SYSTEM_COMPARISON(entityManager, self, sprite_component_layer_compare);

    REGISTER_SYSTEM_HANDLER(MESSAGE_DAMAGE, sprite_system_on_damage);
}

void sprite_system_update(SpriteSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        SpriteComponent* sprite = (SpriteComponent*)GET_SYSTEM_COMPONENT(i);

        REQUIRED_COMPONENTS(sprite);

        if (sprite->redTimer > 0.f) {
            sprite->redTimer -= globals.time.delta;
        }
    }
}

void sprite_system_render(SpriteSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    component_list_insertion_sort(components);

    for (u32 i = 0; i < components->count; ++i) {
        Entity entity = GET_ENTITY(i);
        SpriteComponent* sprite = (SpriteComponent*)GET_SYSTEM_COMPONENT(i);

        TransformComponent* transform = (TransformComponent*)GET_COMPONENT(entity, COMPONENT_TRANSFORM);

        REQUIRED_COMPONENTS(transform, sprite);

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

void sprite_system_on_damage(AspectSystem* system, Entity entity, Message message) {
    SpriteSystem* self = (SpriteSystem*)system;

    SpriteComponent* sprite =
        (SpriteComponent*)GET_COMPONENT(entity, COMPONENT_SPRITE);

    REQUIRED_COMPONENTS(sprite);

    sprite->redTimer = 0.05f;
}