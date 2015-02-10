#ifndef RUNNER_ATLAS_H
#define RUNNER_ATLAS_H

#include "core.h"
#include "hashtable.h"

typedef struct sprite_frame_t {
    Rect frame;
    Vec2 offset;
} SpriteFrame;

typedef struct atlas_t {
    SDL_Texture* texture;
    Hashtable frameTable;
} Atlas;

void atlases_init();
void atlases_terminate();

void atlas_free(Atlas* self);

void atlas_load(const char* name);
void atlas_unload(const char* name);
Atlas* atlas_get(const char* name);
SpriteFrame* atlas_get_frame(Atlas* self, const char* spriteName);

#endif