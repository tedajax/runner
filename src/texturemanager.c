#include "texturemanager.h"

void texture_manager_init(TextureManager* self, const char* rootDir) {
    u32 rootLen = strlen(rootDir);
    self->rootDir = (char*)calloc(rootLen + 1, sizeof(char));
    strcpy(self->rootDir, rootDir);

    hashtable_init(&self->textureTable, 128, _free_void_sdl_texture);
}

void texture_manager_free(TextureManager* self) {
    hashtable_free(&self->textureTable);
    free(self->rootDir);
}

bool texture_manager_load(TextureManager* self, SDL_Renderer* renderer, const char* filename) {
    char* fullPath = (char*)calloc(256, sizeof(char));
    _path_concat(self->rootDir, filename, fullPath);

    SDL_Surface* loadedSurface = IMG_Load(fullPath);
    if (!loadedSurface) {
        free(fullPath);
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

    if (!texture) {
        SDL_FreeSurface(loadedSurface);
        free(fullPath);
        return false;
    }

    hashtable_insert(&self->textureTable, filename, texture);

    SDL_FreeSurface(loadedSurface);
    free(fullPath);

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

void _path_concat(const char* p1, const char* p2, char* dest) {
    u32 l1 = strlen(p1);

    strcpy(dest, p1);
    dest[l1] = '/';
    strcpy(&dest[l1 + 1], p2);
}

void _free_void_sdl_texture(void* ptr) {
    SDL_DestroyTexture((SDL_Texture*)ptr);
}