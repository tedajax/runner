#include "globals.h"

Globals globals;

void globals_init() {
    globals.randomSeed = 0;

    globals.screen.width = 0;
    globals.screen.height = 0;

    game_time_initialize(&globals.time);

    globals.gravity.x = 0.f;
    globals.gravity.y = 1000.f;

    globals.window = NULL;
    globals.renderer = NULL;

    globals.bulletTexture = NULL;
}