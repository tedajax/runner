#ifndef RUNNER_TEXTURE_MANAGER_H
#define RUNNER_TEXTURE_MANAGER_H

#include "types.h"
#include "hashtable.h"
#include "path.h"

#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct texture_manager_t {
    Hashtable textureTable;
    char* rootDir;
} TextureManager;

void texture_manager_init(TextureManager* self, const char* rootDir);
void texture_manager_free(TextureManager* self);
bool texture_manager_load(TextureManager* self, SDL_Renderer* renderer, const char* filename);
bool texture_manager_unload(TextureManager* self, const char* filename);
bool texture_manager_unload_all(TextureManager* self);
SDL_Texture* texture_manager_get(TextureManager* self, const char* name);

//TODO: probably make a path handling system
//Private
void _free_void_sdl_texture(void* ptr);

#endif