#ifndef RUNNER_APPLICATION_H
#define RUNNER_APPLICATION_H

#include <SDL2/SDL.h>

#include <time.h>

#include "globals.h"
#include "types.h"
#include "debug.h"
#include "log.h"
#include "window.h"

int app_run(int argc, char* argv[]);

bool app_initialize();
void app_terminate();

#endif