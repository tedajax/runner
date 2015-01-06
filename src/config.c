#include "config.h"

void config_type_free(Config* self) {
    free(self->path);
    free(self);
}

void config_type_free_void(void* pself) {
    config_type_free((Config*)pself);
}

void config_type_update_mtime(Config* self) {
    time_t mtime = config_get_mtime(self->path);
    
    if (mtime > self->lastMTime) {
        // TODO: reload config, propagate changes to relevant config types
        self->lastMTime = mtime;
    }
}

void config_init() {
    hashtable_init(&configTable, 32, config_type_free_void);

    rootDir = "assets/data/";
}

void config_terminate() {
    hashtable_free(&configTable);
    rootDir = NULL;
}

void config_load(const char* filename) {
    ASSERT(rootDir, "Call config_init() before attempting to load configs!");

    char fullPath[256];
    IF_DEBUG(bool concatResult = )path_concat(rootDir, filename, fullPath, 256);
    ASSERT(concatResult, "Failed to concatenate paths, destination string not long enough.");
    
    IF_DEBUG(void* element = hashtable_get(&configTable, filename);)
    ASSERT(element == NULL, "Config already loaded.");

    Config* newConfig = CALLOC(1, Config);
    ini_load(&newConfig->data, fullPath);
    newConfig->path = CALLOC(strlen(fullPath) + 1, char);
    strcpy(newConfig->path, fullPath);
    newConfig->lastMTime = config_get_mtime(newConfig->path);

    hashtable_insert(&configTable, filename, (void*)newConfig);
}

Config* config_get(const char* name) {
    Config* cfg = (Config*)hashtable_get(&configTable, name);
    ASSERT(cfg, "Config has not been loaded");
    return cfg;
}

time_t config_get_mtime(const char* path) {
    struct stat buffer;

    if (stat(path, &buffer) != 0) {
        ASSERT(false, "TODO: handle this gracefully.");
        return 0;
    }

    return buffer.st_mtime;
}

CONFIG_GET_AT_PROTO_NAMED(char*, string) {
    return ini_get_string_at(&self->data, )
}

CONFIG_TRY_GET_AT_PROTO_NAMED(char*, string) {

}

CONFIG_GET_AT_PROTO(int) {

}