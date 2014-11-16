#ifndef RUNNER_INPUT_H
#define RUNNER_INPUT_H

#include <SDL2/SDL.h>

#include "core.h"

static bool oldKeyStates[SDL_NUM_SCANCODES];
static bool newKeyStates[SDL_NUM_SCANCODES];

void input_initialize();

void input_update();
void input_handle_event(SDL_KeyboardEvent* event);

bool input_key(u32 scancode);
bool input_key_down(u32 scancode);
bool input_key_up(u32 scancode);

#endif