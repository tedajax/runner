#include "config.h"

void config_type_free(Config* self) {
    hashtable_free(&self->typeConfigs);
    free(self->path);
    free(self);
}

void config_type_free_void(void* pself) {
    config_type_free((Config*)pself);
}

bool config_type_update_mtime(Config* self) {
    time_t mtime = config_get_mtime(self->path);
    
    if (mtime > self->lastMTime) {
        // TODO: reload config, propagate changes to relevant config types
        self->lastMTime = mtime;
        return true;
    }

    return false;
}

void config_type_reload(Config* self) {
    ini_init(&self->data);
    ini_load(&self->data, self->path);

    TypeConfig* typeConfigs[128];
    u32 count = (u32)hashtable_get_all(&self->typeConfigs, (void**)typeConfigs, 128);

    for (u32 i = 0; i < count; ++i) {
        TypeConfig* typeConfig = typeConfigs[i];
        DESERIALIZE(typeConfig->type, typeConfig, self, typeConfig->tableName);
    }
}

void config_init() {
    hashtable_init(&configTable, 32, config_type_free_void);

    rootDir = "assets/data/";

    REGISTER_DESERIALIZE_FUNCTION(TYPE_CONFIG_COLLIDER, collider_config_deserialize);
    REGISTER_DESERIALIZE_FUNCTION(TYPE_CONFIG_BULLET_SOURCE, bullet_source_config_deserialize);
    REGISTER_DESERIALIZE_FUNCTION(TYPE_CONFIG_TWEEN, tween_config_deserialize);
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

    hashtable_init(&newConfig->typeConfigs, 32, free);
    hashtable_insert(&configTable, filename, (void*)newConfig);
}

void config_reload_all() {
    Config* configs[128];
    u32 count = (u32)hashtable_get_all(&configTable, (void**)configs, 128);

    for (u32 i = 0; i < count; ++i) {
        Config* config = (Config*)configs[i];

        if (config_type_update_mtime(config)) {
            config_type_reload(config);
        }
    }
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
    return ini_get_string_at(&self->data, section, key, index);
}

CONFIG_TRY_GET_AT_PROTO_NAMED(char*, string) {
    return ini_try_get_string_at(&self->data, section, key, index, defaultValue);
}

CONFIG_GET_AT_PROTO(int) {
    return ini_get_int_at(&self->data, section, key, index);
}

CONFIG_TRY_GET_AT_PROTO(int) {
    return ini_try_get_int_at(&self->data, section, key, index, defaultValue);
}

CONFIG_GET_AT_PROTO(float) {
    return ini_get_float_at(&self->data, section, key, index);
}

CONFIG_TRY_GET_AT_PROTO(float) {
    return ini_try_get_float_at(&self->data, section, key, index, defaultValue);
}

CONFIG_GET_AT_PROTO(bool) {
    return ini_get_bool_at(&self->data, section, key, index);
}

CONFIG_TRY_GET_AT_PROTO(bool) {
    return ini_try_get_bool_at(&self->data, section, key, index, defaultValue);
}

CONFIG_GET_AT_PROTO(Vec2) {
    f32 x = ini_get_float_at(&self->data, section, key, index * 2 + 0);
    f32 y = ini_get_float_at(&self->data, section, key, index * 2 + 1);
    return vec2_init(x, y);
}

CONFIG_TRY_GET_AT_PROTO(Vec2) {
    f32 x = ini_try_get_float_at(&self->data, section, key, index * 2 + 0, defaultValue.x);
    f32 y = ini_try_get_float_at(&self->data, section, key, index * 2 + 1, defaultValue.y);
    return vec2_init(x, y);
}

CONFIG_GET_AT_PROTO(Range) {
    f32 min = ini_get_float_at(&self->data, section, key, index * 2 + 0);
    f32 max = ini_get_float_at(&self->data, section, key, index * 2 + 1);
    Range r;
    range_init(&r, min, max);
    return r;
}

CONFIG_TRY_GET_AT_PROTO(Range) {
    f32 min = ini_try_get_float_at(&self->data, section, key, index * 2 + 0, defaultValue.min);
    f32 max = ini_try_get_float_at(&self->data, section, key, index * 2 + 1, defaultValue.max);
    Range r;
    range_init(&r, min, max);
    return r;
}

CONFIG_GET_AT_PROTO(dynf32) {
    dynf32 result;
    dynf32_zero(&result);

    char* str = config_get_string_at(self, section, key, index);

    char* endptr;
    result.value = strtof(str, &endptr);

    if (*endptr == '\0') {
        result.type = DYN_F32_VALUE;
        return result;
    }

    result.tweenConfig = CONFIG_GET_AT(TweenConfig)(self, section, key, index);
    result.tween = NULL;
    result.type = DYN_F32_TWEEN;
    return result;
}

CONFIG_TRY_GET_AT_PROTO(dynf32) {
    if (!config_try_get_string_at(self, section, key, index, NULL)) { return defaultValue; }

    return CONFIG_GET_AT(dynf32)(self, section, key, index);
}

CONFIG_TYPE_CONFIG_IMPLEMENTATIONS(ColliderConfig, TYPE_CONFIG_COLLIDER);
CONFIG_TYPE_CONFIG_IMPLEMENTATIONS(BulletSourceConfig, TYPE_CONFIG_BULLET_SOURCE);
CONFIG_TYPE_CONFIG_IMPLEMENTATIONS(TweenConfig, TYPE_CONFIG_TWEEN);
