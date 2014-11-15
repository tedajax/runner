#include "window.h"

SDL_Window* window_create(const char* title,
    int positionX,
    int positionY,
    int width,
    int height,
    u32 flags) {

    SDL_Window* window = SDL_CreateWindow(title,
        positionX,
        positionY,
        width,
        height,
        flags);

    if (!window) {
        log_error("SDL", SDL_GetError());
        return NULL;
    }

    return window;
}