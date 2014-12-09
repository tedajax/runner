#include "texturemanager.h"

void texture_manager_init(TextureManager* self, const char* rootDir) {
    u32 rootLen = (u32)strlen(rootDir);
    self->rootDir = (char*)calloc(rootLen + 1, sizeof(char));
    strcpy(self->rootDir, rootDir);

    hashtable_init(&self->textureTable, 128, _free_void_sdl_texture);
}

void texture_manager_free(TextureManager* self) {
    hashtable_free(&self->textureTable);
    free(self->rootDir);
}

bool texture_manager_load(TextureManager* self, SDL_Renderer* renderer, const char* filename) {
    char fullPath[256];
    IF_DEBUG(bool concatResult = )path_concat(self->rootDir, filename, fullPath, 256);
    ASSERT(concatResult, "Failed to concatenate paths, destination string not long enough.");

    SDL_Surface* loadedSurface = IMG_Load(fullPath);
    if (!loadedSurface) {
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

    if (!texture) {
        SDL_FreeSurface(loadedSurface);
        return false;
    }

    hashtable_insert(&self->textureTable, filename, texture);

    SDL_FreeSurface(loadedSurface);

    return true;
}

bool texture_manager_unload(TextureManager* self, const char* filename) {
    void* result = hashtable_remove(&self->textureTable, filename);

    return result != NULL;
}

bool texture_manager_unload_all(TextureManager* self) {
    hashtable_clear(&self->textureTable);
    return true;
}

SDL_Texture* texture_manager_get(TextureManager* self, const char* name) {
    return (SDL_Texture*)hashtable_get(&self->textureTable, name);
}

void _free_void_sdl_texture(void* ptr) {
    SDL_DestroyTexture((SDL_Texture*)ptr);
}