#include "prefab.h"

void prefab_init(Prefab* self, char* filename) {
    self->config = config_get(filename);
    self->componentNames = NULL;
    self->componentCount = 0;
}

void prefab_free(Prefab* self) {
    if (self->componentNames) {
        free(self->componentNames);
    }
}

void prefab_reload(Prefab* self) {
    int count = config_get_array_count(self->config, CONFIG_DEFAULT_SECTION, "components");
    self->componentCount = (u32)count;
    
    if (self->componentNames) {
        free(self->componentNames);
    }

    self->componentNames = CALLOC(self->componentCount, PrefabString);
    for (u32 i = 0; i < self->componentCount; ++i) {
        char* name = config_get_string_at(self->config, CONFIG_DEFAULT_SECTION, "components", i);
        strncpy(self->componentNames[i], name, PREFAB_MAX_COMPONENT_NAME_LENGTH);
    }
}

void prefab_instantiate(Prefab* self) {
    prefab_instantiate_at(self, vec2_zero(), 0.f);
}

void prefab_instantiate_at(Prefab* self, Vec2 position, f32 rotation) {
    
}