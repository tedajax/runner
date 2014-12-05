#ifndef RUNNER_INI_H
#define RUNNER_INI_H

#include <stdlib.h>

#include "types.h"
#include "debug.h"
#include "log.h"

#define INI_MAX_SECTIONS 128
#define INI_MAX_KEYS_PER_SECTION 128

#define INI_MAX_SECTION_NAME_LENGTH 256

#define INI_DEFAULT_SECTION "__global"

typedef int(*str_compare_f)(char*, char*);

extern str_compare_f ini_strcmp;

typedef struct ini_tree_kvp_t {
    u64 key;
    char* value;
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

int ini_section_index(Ini* self, char* section);
int ini_index(Ini* self, char* section, char* key);

bool ini_section_exists(Ini* self, char* section);
bool ini_exists(Ini* self, char* section, char* key);

int ini_section_count(Ini* self);
int ini_keys_count(Ini* self, char* section);

int ini_get_int(Ini* self, char* section, char* key);
float ini_get_float(Ini* self, char* section, char* key);
bool ini_get_bool(Ini* self, char* section, char* key);
char* ini_get_string(Ini* self, char* section, char* key);

int ini_try_get_int(Ini* self, char* section, char* key, int defaultVal);
float ini_try_get_float(Ini* self, char* section, char* key, float defaultVal);
bool ini_try_get_bool(Ini* self, char* section, char* key, bool defaultVal);
char* ini_try_get_string(Ini* self, char* section, char* key, char* defaultVal);

void ini_dump(Ini* self, FILE* file);

#endif