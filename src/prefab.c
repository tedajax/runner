#include "prefab.h"
#include "entitymanager.h"
#include "game.h"
#include "directory.h"

ConfigSystem prefabConfigs;
Hashtable prefabTable;

void prefab_system_init(EntityManager* entityManager, const char* prefabRoot) {
    config_system_init(&prefabConfigs, (char*)prefabRoot);
    hashtable_init(&prefabTable, 32, prefab_free_void);

    Directory* prefabDir = directory_open(prefabRoot);
    FileDescriptor* currentFile;
    while ((currentFile = directory_next(prefabDir))) {
        config_system_load(&prefabConfigs, currentFile->filename);
        Config* cfg = config_system_get(&prefabConfigs, currentFile->filename);
        Prefab* newPrefab = CALLOC(1, Prefab);
        newPrefab->config = cfg;
        prefab_reload(newPrefab);
        hashtable_insert(&prefabTable, currentFile->filename, (void*)newPrefab);
    }
    directory_close(prefabDir);
}

void prefab_system_terminate() {
    config_system_terminate(&prefabConfigs);
    hashtable_free_contents(&prefabTable);
}

Prefab* prefab_new(char* filename) {
    Prefab* self = CALLOC(1, Prefab);
    prefab_init(self, filename);
    return self;
}

void prefab_init(Prefab* self, char* filename) {
    self->config = config_system_get(&prefabConfigs, filename);
    component_batch_zero(&self->components);
}

void prefab_free(Prefab* self) {
    component_batch_free(&self->components);
    free(self);
}

void prefab_free_void(void* self) {
    prefab_free((Prefab*)self);
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

Prefab* prefab_get(char* name) {
    return (Prefab*)hashtable_get(&prefabTable, name);
}

void prefab_instantiate(Prefab* self) {
    prefab_instantiate_at(self, vec2_zero(), 0.f);
}

void prefab_instantiate_at(Prefab* self, Vec2 position, f32 rotation) {
    entities_instantiate_prefab(globals.game->entityManager, self, position, rotation);
}