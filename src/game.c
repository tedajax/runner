#include "game.h"
#include "prefab.h"

#include <SDL2/SDL_image.h>

bool drawCollision = false;

void game_debug_keys(Game* self);

void game_init(Game* self) {  
    globals.game = self;

    component_system_init();

    textures_init("assets/textures");

    textures_load("player_ship.png");
    textures_load("player_bullet_1.png");
    textures_load("player_bullet_2.png");
    textures_load("bg_dark_purple.png");
    textures_load("enemy_red_1.png");
    textures_load("fireparticle.png");
    textures_load("bullet.png");    

    self->entityManager = entity_manager_new();
    health_system_init(&self->healthSystem, self->entityManager);
    sprite_system_init(&self->spriteSystem, self->entityManager);
    movement_system_init(&self->movementSystem, self->entityManager);
    gravity_system_init(&self->gravitySystem, self->entityManager);
    controller_system_init(&self->controllerSystem, self->entityManager);
    bullet_controller_system_init(&self->bulletControllerSystem, self->entityManager);
    bg_manager_system_init(&self->bgManagerSystem, self->entityManager);
    enemy_system_init(&self->enemySystem, self->entityManager);
    collision_system_init(&self->collisionSystem, self->entityManager);
    lua_system_init(&self->luaSystem, self->entityManager);

    int bgTextureWidth;
    int bgTextureHeight;
    SDL_QueryTexture(textures_get("bg_dark_purple.png"), NULL, NULL, &bgTextureWidth, &bgTextureHeight);

    Entity bgManagerEntity = entity_create_bg_manager(self->entityManager,
        (u32)bgTextureWidth,
        (u32)bgTextureHeight);

    BgManagerComponent* bgManager = 
        (BgManagerComponent*)entities_get_component(self->entityManager,
        COMPONENT_BG_MANAGER,
        bgManagerEntity);

    for (u32 i = 0; i < bgManager->capacity; ++i) {
        Entity tile = entity_create_bg_tile(self->entityManager,
            textures_get("bg_dark_purple.png"));

        TransformComponent* tx =
            (TransformComponent*)entities_get_component(self->entityManager,
            COMPONENT_TRANSFORM,
            tile);

        bg_manager_component_add_entity(bgManager, tx);
    }

    self->player = entity_create_player(self->entityManager,
        vec2_init(32.f, 320.f),
        textures_get("player_ship.png"));

    globals.player = self->player;

    SDL_Rect cameraConstraints = {
        32,
        32,
        400,
        563
    };

    for (u32 i = 0; i < 50; ++i) {
        entity_create_basic_enemy(self->entityManager, vec2_init(i * 100.f, randf((f32)globals.world.height - 100.f)));
    }

    camera_init(&globals.camera, NULL, &cameraConstraints);

    {
        debug_hud_init(&self->debugHud, "assets/fonts/terminus.ttf", 12);

        DebugHudWatch* fpsWatch = debug_hud_add_watch(&self->debugHud, "FPS", WATCH_TYPE_INT, &globals.time.fps);
        debug_hud_watch_set_warnings(fpsWatch, false, 120, 60);

        DebugHudWatch* frameTimeWatch = debug_hud_add_watch(&self->debugHud, "Frame MS", WATCH_TYPE_INT, &globals.time.per_frame_ms);
        debug_hud_watch_set_warnings(frameTimeWatch, true, 14, 16);

        debug_hud_add_watch(&self->debugHud, "Timescale", WATCH_TYPE_FLOAT, &globals.time.timescale);

        DebugHudWatch* entityWatch = debug_hud_add_watch(&self->debugHud, "Entities", WATCH_TYPE_INT, &self->entityManager->entities.count);
        debug_hud_watch_set_warnings(entityWatch, true, 900, 1000);

        debug_hud_add_watch(&self->debugHud, "Level Position", WATCH_TYPE_FLOAT, &globals.levelPosition);

        DebugHudWatch* tweenWatch = debug_hud_add_watch(&self->debugHud, "Tweens", WATCH_TYPE_INT, &globals.tweens.count);
        debug_hud_watch_set_warnings(tweenWatch, true, 2500, 4000);
    }

    prefab_system_init(self->entityManager, "assets/prefabs");
}

void game_quit(Game* self) {
    prefab_system_terminate();
    entity_manager_free(self->entityManager);
    debug_hud_free(&self->debugHud);
    component_system_terminate();
}

void game_start(Game* self) {
    bg_manager_system_start(&self->bgManagerSystem);
    collision_system_start(&self->collisionSystem);
    lua_system_start(&self->luaSystem);
}

void game_update(Game* self) {
    globals.levelPosition += globals.scrollSpeed * globals.time.delta;

    health_system_update(&self->healthSystem);
    sprite_system_update(&self->spriteSystem);
    controller_system_update(&self->controllerSystem);
    bullet_controller_system_update(&self->bulletControllerSystem);
    gravity_system_update(&self->gravitySystem);
    enemy_system_update(&self->enemySystem);
    movement_system_update(&self->movementSystem);
    bg_manager_system_update(&self->bgManagerSystem);
    lua_system_update(&self->luaSystem);

    profiler_tick("collision");
    collision_system_update(&self->collisionSystem);
    profiler_tock("collision");

    camera_update(&globals.camera);

    debug_hud_update_surfaces(&self->debugHud, globals.renderer);
    tween_manager_update(&globals.tweens, globals.time.delta);

    game_debug_keys(self);
}

void game_debug_keys(Game* self) {
    if (input_key_down(SDL_SCANCODE_F1)) {
        debug_hud_dump(&self->debugHud, stdout);
    }

    if (input_key_down(SDL_SCANCODE_F2)) {
        profiler_dump(stdout, false);
    }

    if (input_key_down(SDL_SCANCODE_F3)) {
        drawCollision = !drawCollision;
    }

    if (input_key_down(SDL_SCANCODE_MINUS)) {
        globals.time.timescale -= 0.1f;
    }

    if (input_key_down(SDL_SCANCODE_EQUALS)) {
        globals.time.timescale += 0.1f;
    }

    if (input_key_down(SDL_SCANCODE_F12)) {
        DEBUG_BREAKPOINT();
    }
}

void game_render(Game* self) {
    sprite_system_render(&self->spriteSystem);
    if (drawCollision) {
        collision_system_render(&self->collisionSystem);
    }
    lua_system_render(&self->luaSystem);

    debug_hud_render(&self->debugHud, globals.renderer, 5, 5);
}

void game_frame_end(Game* self) {
    entities_update(self->entityManager);
}