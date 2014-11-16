#ifndef RUNNER_APPLICATION_H
#define RUNNER_APPLICATION_H

#include <SDL2/SDL.h>

#include <time.h>

#include "core.h"
#include "window.h"
#include "input.h"
#include "gametime.h"
#include "game.h"

typedef struct app_t {
    bool shouldQuit;
    Game game;
} App;


//Public:
int app_run(int argc, char* argv[]);
void app_quit(App* self);


//Private:
bool _app_initialize(App* self);
void _app_terminate(App* self);

void _app_handle_event(App* self, SDL_Event* event);

void _app_update(App* self);
void _app_render(App* self);

App* _app_new();
void _app_free(App* self);

void _app_print_fps();

#endif