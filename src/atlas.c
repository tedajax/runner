#include "atlas.h"

void atlas_free_void(void* pself);

Hashtable atlasTable;

void atlases_init() {
    hashtable_init(&atlasTable, 64, atlas_free_void);
}

void atlases_terminate() {
    hashtable_free_contents(&atlasTable);
}

void atlas_free(Atlas* self) {
    hashtable_free_contents(&self->frameTable);
}

void atlas_free_void(void* pself) {
    Atlas* self = (Atlas*)pself;
    atlas_free(self);
    free(self);
}

void atlas_load(const char* textureFile, const char* dataFile, const char* name) {
    Atlas* self = CALLOC(1, Atlas);
    
    hashtable_init(&self->frameTable, 32, NULL);

    self->texture = textures_get(textureFile);
    ASSERT(self->texture, "Could not find specified texture.");

    //TODO Load data file, create frames, load them in

    hashtable_insert(&atlasTable, name, self);
}

void atlas_unload(const char* name) {
    hashtable_remove(&atlasTable, name);
}

Atlas* atlas_get(const char* name) {
    Atlas* result = (Atlas*)hashtable_get(&atlasTable, name);
    ASSERT(result, "Could not find atlas.");
    return result;
}

SpriteFrame* atlas_get_frame(Atlas* self, const char* spriteName) {
    return (SpriteFrame*)hashtable_get(&self->frameTable, spriteName);
}
