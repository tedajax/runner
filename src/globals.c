#include "globals.h"

Globals globals;

void globals_init() {
    globals.randomSeed = 0;

    globals.screen.width = 0;
    globals.screen.height = 0;

    globals.world.width = 0;
    globals.world.height = 0;

    game_time_initialize(&globals.time);

    globals.gravity.x = 0.f;
    globals.gravity.y = 1000.f;

    globals.window = NULL;
    globals.renderer = NULL;
}

void textures_init(const char* rootDir) { texture_manager_init(&globals.textures, rootDir); }
void textures_free() { texture_manager_free(&globals.textures); }
bool textures_load(const char* filename) { return texture_manager_load(&globals.textures, globals.renderer, filename); }
bool textures_unload(const char* filename) { return texture_manager_unload(&globals.textures, filename); }
bool textures_unload_all() { return texture_manager_unload_all(&globals.textures); }
SDL_Texture* textures_get(const char* name) { return texture_manager_get(&globals.textures, name); }