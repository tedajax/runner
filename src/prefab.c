#include "prefab.h"
#include "entitymanager.h"
#include "game.h"

void prefab_init(Prefab* self, char* filename) {
    self->config = config_get(filename);
    component_batch_zero(&self->components);
}

void prefab_free(Prefab* self) {
    if (self->components.components) {
        free(self->components.components);
    }
}

void prefab_reload(Prefab* self) {
    u32 count = (u32)config_get_array_count(self->config, CONFIG_DEFAULT_SECTION, "components");
    component_batch_init(&self->components, count);

    for (u32 i = 0; i < count; ++i) {
        char* name = config_get_string_at(self->config, CONFIG_DEFAULT_SECTION, "components", i);
        Component* component = component_deserialize(self->config, name);
        component_batch_add(&self->components, component);
    }
}

void prefab_instantiate(Prefab* self) {
    prefab_instantiate_at(self, vec2_zero(), 0.f);
}

void prefab_instantiate_at(Prefab* self, Vec2 position, f32 rotation) {
    entities_instantiate_prefab(globals.game->entityManager, self, position, rotation);
}