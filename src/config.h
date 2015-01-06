#ifndef RUNNER_CONFIG_H
#define RUNNER_CONFIG_H

#include "ini.h"
#include "hashtable.h"
#include "path.h"
#include "inline.h"

#include "configtypes.h"

#include <sys/types.h>
#include <sys/stat.h>

typedef struct config_t {
    Ini data;
    char* path;
    time_t lastMTime; // last modification time of the file
} Config;

void config_type_free(Config* self);
void config_type_free_void(void* pself);
void config_type_update_mtime(Config* self);

char* rootDir;
Hashtable configTable;

void config_init();
void config_terminate();
void config_load(const char* filename);
Config* config_get(const char* name);

#define CONFIG_GET_AT(name) config_get_##name##_at
#define CONFIG_TRY_GET_AT(name) config_try_get_##name##_at

#define CONFIG_GET(name) config_get_##name
#define CONFIG_TRY_GET(name) config_try_get_##name

#define CONFIG_GET_AT_PROTO_NAMED(type, name) type CONFIG_GET_AT(name)(Config* self, const char* section, const char* key, u32 index)
#define CONFIG_TRY_GET_AT_PROTO_NAMED(type, name) type CONFIG_TRY_GET_AT(name)(Config* self, const char* section, const char* key, u32 index, type defaultValue)

#define CONFIG_GET_AT_PROTO(type) CONFIG_GET_AT_PROTO_NAMED(type, type)
#define CONFIG_TRY_GET_AT_PROTO(type) CONFIG_TRY_GET_AT_PROTO_NAMED(type, type)

#define CONFIG_GET_PROTO_NAMED(type, name) inline type CONFIG_GET(name)(Config* self, const char* section, const char* key) { return CONFIG_GET_AT(name)(self, section, key, 0); }
#define CONFIG_TRY_GET_PROTO_NAMED(type, name) inline type CONFIG_TRY_GET(name)(Config* self, const char* section, const char* key, type defaultValue) { return CONFIG_TRY_GET_AT(name)(self, section, key, 0, defaultValue); }

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
//CONFIG_REGISTER_TYPE(float);
//CONFIG_REGISTER_TYPE(bool);
//CONFIG_REGISTER_TYPE(Vec2);
//CONFIG_REGISTER_TYPE(Range);
//CONFIG_REGISTER_TYPE_NAMED(char*, string);

time_t config_get_mtime(const char* path);

#endif