#include "input.h"

// bool oldKeyStates[SDL_NUM_SCANCODES];
// bool newKeyStates[SDL_NUM_SCANCODES];

void input_initialize() {
    for (u32 i = 0; i < SDL_NUM_SCANCODES; ++i) {
        oldKeyStates[i] = false;
        newKeyStates[i] = false;
    }
}

void input_update() {
    memcpy(&oldKeyStates[0], &newKeyStates[0], SDL_NUM_SCANCODES);
}

void input_handle_event(SDL_KeyboardEvent* event) {
    bool down = (event->type == SDL_KEYDOWN);
    newKeyStates[event->keysym.scancode] = down;
}

bool input_key(u32 scancode) {
    ASSERT(scancode < SDL_NUM_SCANCODES, "Scancode out of range.");
    
    return newKeyStates[scancode];
}

bool input_key_down(u32 scancode) {
    ASSERT(scancode < SDL_NUM_SCANCODES, "Scancode out of range.");

    return newKeyStates[scancode] && !oldKeyStates[scancode];
}

bool input_key_up(u32 scancode) {
    ASSERT(scancode < SDL_NUM_SCANCODES, "Scancode out of range.");

    return !newKeyStates[scancode] && oldKeyStates[scancode];
}