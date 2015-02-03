#include "prefab.h"

void prefab_init(Prefab* self, char* filename) {
    //TODO root pathing
    self->path = filename;
    self->lastMTime = 0;
}

bool prefab_update_mtime(Prefab* self) {
    struct stat buffer;

    if (stat(self->path, &buffer) != 0) {
        return false;
    }

    time_t mtime = buffer.st_mtime;
    if (mtime > self->lastMTime) {
        self->lastMTime = mtime;
        return true;
    }

    return false;
}

bool prefab_check_and_reload(Prefab* self) {
    if (prefab_update_mtime(self)) {
        // TODO: reload things
        return true;
    }
    return false;
}

void prefab_instantiate(Prefab* self) {
    prefab_instantiate_at(self, vec2_zero(), 0.f);
}

void prefab_instantiate_at(Prefab* self, Vec2 position, f32 rotation) {

}