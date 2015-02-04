#ifndef RUNNER_CONFIG_H
#define RUNNER_CONFIG_H

#include "ini.h"
#include "hashtable.h"
#include "path.h"
#include "inline.h"
#include "algebra.h"
#include "range.h"

#include "configtypes.h"

#include <sys/types.h>
#include <sys/stat.h>

#define CONFIG_DEFAULT_SECTION INI_DEFAULT_SECTION

typedef struct config_t {
    Ini data;
    Hashtable typeConfigs;
    char* path;
    time_t lastMTime; // last modification time of the file
} Config;

void config_type_free(Config* self);
void config_type_free_void(void* pself);
bool config_type_update_mtime(Config* self);
void config_type_reload(Config* self);

char* rootDir;
Hashtable configTable;

void config_init();
void config_terminate();
void config_load(const char* filename);
void config_reload_all();
Config* config_get(const char* name);
time_t config_get_mtime(const char* path);

static inline config_get_array_count(Config* self, const char* section, const char* key) { return ini_get_array_count(&self->data, section, key); }

#define STR_ENUM_CMP(str, enumval) (strcmp(str, #enumval) == 0)

#define CONFIG_GET_AT(name) config_get_##name##_at
#define CONFIG_TRY_GET_AT(name) config_try_get_##name##_at

#define CONFIG_GET(name) config_get_##name
#define CONFIG_TRY_GET(name) config_try_get_##name

#define CONFIG_GET_AT_PROTO_NAMED(type, name) type CONFIG_GET_AT(name)(Config* self, const char* section, const char* key, u32 index)
#define CONFIG_TRY_GET_AT_PROTO_NAMED(type, name) type CONFIG_TRY_GET_AT(name)(Config* self, const char* section, const char* key, u32 index, type defaultValue)

#define CONFIG_GET_AT_PROTO(type) CONFIG_GET_AT_PROTO_NAMED(type, type)
#define CONFIG_TRY_GET_AT_PROTO(type) CONFIG_TRY_GET_AT_PROTO_NAMED(type, type)

#define CONFIG_GET_PROTO_NAMED(type, name) static inline type CONFIG_GET(name)(Config* self, const char* section, const char* key) { return CONFIG_GET_AT(name)(self, section, key, 0); }
#define CONFIG_TRY_GET_PROTO_NAMED(type, name) static inline type CONFIG_TRY_GET(name)(Config* self, const char* section, const char* key, type defaultValue) { return CONFIG_TRY_GET_AT(name)(self, section, key, 0, defaultValue); }

#define CONFIG_GET_PROTO(type) CONFIG_GET_PROTO_NAMED(type, type)
#define CONFIG_TRY_GET_PROTO(type) CONFIG_TRY_GET_PROTO_NAMED(type, type)

// Registers get functions using type in name (e.g. config_get_int, config_try_get_int, etc...)
#define CONFIG_REGISTER_TYPE(type)  \
    CONFIG_GET_AT_PROTO(type);CONFIG_TRY_GET_AT_PROTO(type);CONFIG_GET_PROTO(type);CONFIG_TRY_GET_PROTO(type)

// Registers get functions using name in the name and returning type, useful for types like char* because
// * can't be used in a function name (e.g. CONFIG_REGISTER_TYPE_NAMED(char*, string) would yield char* config_get_string, etc...)
#define CONFIG_REGISTER_TYPE_NAMED(type, name) \
    CONFIG_GET_AT_PROTO_NAMED(type, name);CONFIG_TRY_GET_AT_PROTO_NAMED(type, name);CONFIG_GET_PROTO_NAMED(type, name);CONFIG_TRY_GET_PROTO_NAMED(type, name)

CONFIG_REGISTER_TYPE(int);
CONFIG_REGISTER_TYPE(float);
CONFIG_REGISTER_TYPE(bool);
CONFIG_REGISTER_TYPE(Vec2);
CONFIG_REGISTER_TYPE(Range);
CONFIG_REGISTER_TYPE(dynf32);
CONFIG_REGISTER_TYPE_NAMED(char*, string);
CONFIG_REGISTER_TYPE_NAMED(ColliderConfig*, ColliderConfig);
CONFIG_REGISTER_TYPE_NAMED(BulletSourceConfig*, BulletSourceConfig);
CONFIG_REGISTER_TYPE_NAMED(TweenConfig*, TweenConfig);
CONFIG_REGISTER_TYPE_NAMED(LevelConfig*, LevelConfig);

#define CONFIG_GET_KEY(charptr, data, section, key, index) \
    charptr = ini_get_string_at(data, section, key, index);

#define CONFIG_TRY_GET_KEY(charptr, data, section, key, index) \
    charptr = ini_try_get_string_at(data, section, key, index, NULL); \
    if (!charptr) { return NULL; }

#define CONFIG_TYPE_CONFIG_GET_AT_BODY(configtype, typeenum, istry)             \
    MULTILINE_MACRO_BEGIN();                                                    \
    char* cfgSection;                                                           \
    if (istry) {                                                                \
        CONFIG_TRY_GET_KEY(cfgSection, &self->data, section, key, index);       \
    } else {                                                                    \
        CONFIG_GET_KEY(cfgSection, &self->data, section, key, index);           \
    }                                                                           \
    TypeConfig* typeConfig = hashtable_get(&self->typeConfigs, cfgSection);     \
    if (typeConfig) {                                                           \
        ASSERT(typeConfig->type == typeenum,                                    \
            "Stored config referenced by name is not proper type.");            \
        return (configtype*)typeConfig;                                         \
    }                                                                           \
    configtype* config = CALLOC(1, configtype);                                 \
    config->super.type = typeenum;                                              \
    config->super.tableName = cfgSection;                                       \
    DESERIALIZE(typeenum, (TypeConfig*)config, self, cfgSection);               \
    hashtable_insert(&self->typeConfigs, cfgSection, config);                   \
    return config;                                                              \
    MULTILINE_MACRO_END();

#define CONFIG_TYPE_CONFIG_IMPLEMENTATIONS(configtype, typeenum)        \
    CONFIG_GET_AT_PROTO_NAMED(configtype*, configtype) {                \
        CONFIG_TYPE_CONFIG_GET_AT_BODY(configtype, typeenum, false);    \
    }                                                                   \
                                                                        \
    CONFIG_TRY_GET_AT_PROTO_NAMED(configtype*, configtype) {            \
        CONFIG_TYPE_CONFIG_GET_AT_BODY(configtype, typeenum, true);     \
    }

/* Hack to get around the fact that bool is actually a macro that expands to _Bool */
#define config_get_bool config_get__Bool
#define config_try_get_bool config_try_get__Bool
#define config_get_at_bool config_get_at__Bool
#define config_try_get_at_bool config_try_get_at__Bool

#endif