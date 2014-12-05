#include "config.h"

void config_init() {
    hashtable_init(&configTable, 32, ini_free_void);

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

    Ini* newConfig = CALLOC(1, Ini);
    ini_load(newConfig, fullPath);

    IF_DEBUG(void* element = hashtable_get(&configTable, filename);)
    ASSERT(element == NULL, "Config already loaded.");

    hashtable_insert(&configTable, filename, (void*)newConfig);
}

Ini* config_get(const char* name) {
    return (Ini*)hashtable_get(&configTable, name);
}