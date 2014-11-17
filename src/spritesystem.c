#include "spritesystem.h"

SpriteSystem* sprite_system_new(EntityManager* entityManager) {
    SpriteSystem* self = (SpriteSystem*)calloc(1, sizeof(SpriteSystem));

    aspect_system_init(&self->super, entityManager, COMPONENT_SPRITE);

    return self;
}

void sprite_system_update(SpriteSystem* self) {
    EntityList entities;
    entity_list_init(&entities, 64);
    aspect_system_entities((AspectSystem*)self, &entities);

    for (u32 i = 0; i < entities.size; ++i) {
        Entity entity = entities.list[i];

        TransformComponent* transform = (TransformComponent*)entities_get_component(
            self->super.entityManager,
            COMPONENT_TRANSFORM,
            &entity);

        SpriteComponent* sprite = (SpriteComponent*)entities_get_component(
            self->super.entityManager,
            COMPONENT_SPRITE,
            &entity);

        if (transform && sprite) {
            // do some things
        }
    }
}

void sprite_system_render(SpriteSystem* self) {
    EntityList entities;
    entity_list_init(&entities, 64);
    aspect_system_entities((AspectSystem*)self, &entities);

    for (u32 i = 0; i < entities.size; ++i) {
        Entity entity = entities.list[i];

        TransformComponent* transform = (TransformComponent*)entities_get_component(
            self->super.entityManager,
            COMPONENT_TRANSFORM,
            &entity);

        SpriteComponent* sprite = (SpriteComponent*)entities_get_component(
            self->super.entityManager,
            COMPONENT_SPRITE,
            &entity);

        if (transform && sprite) {
            if (sprite->texture) {
                SDL_Rect dest;
                sprite_component_destination(sprite, transform, &dest);

                SDL_RenderCopyEx(globals.renderer,
                    sprite->texture,
                    NULL, //source
                    &dest, //destination
                    transform->rotation,
                    NULL,
                    SDL_FLIP_NONE);
            }
        }
    }
}