#include "globals.h"

Globals globals;

void globals_init() {
    globals.randomSeed = 0;

    globals.screen.width = 0;
    globals.screen.height = 0;

    globals.window = NULL;
    globals.renderer = NULL;
}