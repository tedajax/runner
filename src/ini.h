#ifndef RUNNER_INI_H
#define RUNNER_INI_H

#include <stdlib.h>

#include "types.h"
#include "debug.h"
#include "log.h"
#include "inline.h"

#define INI_MAX_SECTIONS 128
#define INI_MAX_KEYS_PER_SECTION 128

#define INI_MAX_SECTION_NAME_LENGTH 256

#define INI_DEFAULT_SECTION "__global"
#define INI_GLOBAL INI_DEFAULT_SECTION

typedef int(*str_compare_f)(const char*, const char*);

extern str_compare_f ini_strcmp;

typedef enum ini_value_type_e {
    INI_VALUE_NUMBER,
    INI_VALUE_STRING,
    INI_VALUE_BOOLEAN,
} IniValueType;

typedef union ini_kvp_value_u {
    char* string;
    f32 number;
    bool boolean;
} IniKvpValue;

typedef struct ini_kvp_value_list_t {
    IniValueType type;
    IniKvpValue* values;
    u32 count;
} IniKvpValueList;

typedef struct ini_tree_kvp_t {
    u64 key;
    IniKvpValueList values;
} IniTreeKvp;

typedef struct ini_tree_t {
    u64 sectionHashes[INI_MAX_SECTIONS];
    IniTreeKvp table[INI_MAX_SECTIONS][INI_MAX_KEYS_PER_SECTION];
    u32 sectionKeyCounts[INI_MAX_SECTIONS];
    u32 sectionCount;
} Ini;

void ini_tree_kvp_clear(IniTreeKvp* self);
void ini_tree_kvp_free(IniTreeKvp* self);
void ini_tree_kvp_set(IniTreeKvp* self, char* key, char* value);

void ini_init(Ini* self);
void ini_load(Ini* self, const char* filename);
void ini_free(Ini* self);
void ini_free_void(void* self);

void ini_add_section(Ini* self, char* section);
void ini_add_key(Ini* self, char* section, char* key, char* value);
void ini_set_key(Ini* self, char* section, char* key, char* value);

int ini_section_index(Ini* self, const char* section);
int ini_index(Ini* self, const char* section, const char* key);

bool ini_section_exists(Ini* self, char* section);
bool ini_exists(Ini* self, char* section, char* key);

int ini_section_count(Ini* self);
int ini_keys_count(Ini* self, char* section);

int ini_try_get_int_at(Ini* self, const char* section, const char* key, u32 index, int defaultVal);
float ini_try_get_float_at(Ini* self, const char* section, const char* key, u32 index, float defaultVal);
bool ini_try_get_bool_at(Ini* self, const char* section, const char* key, u32 index, bool defaultVal);
char* ini_try_get_string_at(Ini* self, const char* section, const char* key, u32 index, char* defaultVal);

int ini_get_int_at(Ini* self, const char* section, const char* key, u32 index);
float ini_get_float_at(Ini* self, const char* section, const char* key, u32 index);
bool ini_get_bool_at(Ini* self, const char* section, const char* key, u32 index);
char* ini_get_string_at(Ini* self, const char* section, const char* key, u32 index);

inline int ini_try_get_int(Ini* self, const char* section, const char* key, int defaultVal) { return ini_try_get_int_at(self, section, key, 0, defaultVal); }
inline float ini_try_get_float(Ini* self, const char* section, const char* key, float defaultVal) { return ini_try_get_float_at(self, section, key, 0, defaultVal); }
inline bool ini_try_get_bool(Ini* self, const char* section, const char* key, bool defaultVal) { return ini_try_get_bool_at(self, section, key, 0, defaultVal); }
inline char* ini_try_get_string(Ini* self, const char* section, const char* key, char* defaultVal) { return ini_try_get_string_at(self, section, key, 0, defaultVal); }

inline int ini_get_int(Ini* self, const char* section, const char* key) { return ini_get_int_at(self, section, key, 0); }
inline float ini_get_float(Ini* self, const char* section, const char* key) { return ini_get_float_at(self, section, key, 0); }
inline bool ini_get_bool(Ini* self, const char* section, const char* key) { return ini_get_bool_at(self, section, key, 0); }
inline char* ini_get_string(Ini* self, const char* section, const char* key) { return ini_get_string_at(self, section, key, 0); }

void ini_dump(Ini* self, FILE* file);

#endif