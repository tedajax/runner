#include "bgmanagersystem.h"

BgManagerSystem* bg_manager_system_new(EntityManager* entityManager) {
    BgManagerSystem* self = CALLOC(1, BgManagerSystem);

    aspect_system_init(&self->super, entityManager, COMPONENT_BG_MANAGER);

    return self;
}

void bg_manager_system_start(BgManagerSystem* self, EntityList* entities) {
    aspect_system_entities((AspectSystem*)self, entities);

    for (u32 i = 0; i < entities->size; ++i) {
        Entity entity = entities->list[i];

        BgManagerComponent* bg =
            (BgManagerComponent*)GET_COMPONENT(entity, COMPONENT_BG_MANAGER);

        if (!bg) {
            continue;
        }

        for (u32 j = 0; j < bg->count; ++j) {
            TransformComponent* tx = bg->transforms[j];

            //todo place correctly based on camera and shit
            f32 x = (f32)((j % bg->columns) * bg->tileWidth);
            f32 y = (f32)((j / bg->columns) * bg->tileHeight);
            vec2_set(&tx->position, x, y);
        }
    }
}

void bg_manager_system_update(BgManagerSystem* self, EntityList* entities) {
    aspect_system_entities((AspectSystem*)self, entities);

    for (u32 i = 0; i < entities->size; ++i) {
        Entity entity = entities->list[i];

        BgManagerComponent* bg =
            (BgManagerComponent*)GET_COMPONENT(entity, COMPONENT_BG_MANAGER);

        if (!bg) {
            continue;
        }

        f32 left = globals.camera.position.x - (f32)bg->tileWidth;
        f32 right = globals.camera.position.x + globals.world.width + (f32)bg->tileWidth;
        f32 top = globals.camera.position.y - (f32)bg->tileHeight;
        f32 bottom = globals.camera.position.y + globals.world.height;

        for (u32 j = 0; j < bg->count; ++j) {
            TransformComponent* tx = bg->transforms[j];

            if (tx->position.x < left) {
                tx->position.x += (f32)(bg->columns * bg->tileWidth);
            } else if (tx->position.x > right) {
                tx->position.x -= (f32)(bg->columns * bg->tileWidth);
            }

            if (tx->position.y < top) {
                tx->position.y += (f32)(bg->rows * bg->tileHeight);
            } else if (tx->position.y > bottom) {
                tx->position.y -= (f32)(bg->rows * bg->tileHeight);
            }
        }
    }
}