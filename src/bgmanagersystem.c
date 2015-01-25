#include "bgmanagersystem.h"

void bg_manager_system_init(BgManagerSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_BG_MANAGER);
}

void bg_manager_system_start(BgManagerSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        BgManagerComponent* bg = (BgManagerComponent*)GET_SYSTEM_COMPONENT(i);

        REQUIRED_COMPONENTS(bg);

        for (u32 j = 0; j < bg->count; ++j) {
            TransformComponent* tx = bg->transforms[j];

            //todo place correctly based on camera and shit
            f32 x = (f32)((j % bg->columns) * bg->tileWidth);
            f32 y = (f32)((j / bg->columns) * bg->tileHeight);
            vec2_set(&tx->position, x, y);
        }
    }
}

void bg_manager_system_update(BgManagerSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        BgManagerComponent* bg = (BgManagerComponent*)GET_SYSTEM_COMPONENT(i);

        REQUIRED_COMPONENTS(bg);

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