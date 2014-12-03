#include "ini.h"

#include <string.h>

// "private" utility functions
u64 _ini_djb2(const char *key);
void _ini_strim(char* s);
void _ini_striml(char* s);
void _ini_strimr(char* s);
bool _ini_isws(char c);

str_compare_f ini_strcmp = strcmp;

void ini_tree_kvp_clear(IniTreeKvp* self) {
    self->key = 0;
    self->value = NULL;
}

void ini_tree_kvp_free(IniTreeKvp* self) {
    free(self->value);
}

void ini_tree_kvp_set(IniTreeKvp* self, char* key, char* value) {
    self->key = _ini_djb2(key);
    self->value = value;
}

void ini_init(Ini* self) {
    self->sectionCount = 0;
    for (u32 i = 0; i < INI_MAX_SECTIONS; ++i) {
        for (u32 j = 0; j < INI_MAX_KEYS_PER_SECTION; ++j) {
            ini_tree_kvp_clear(&self->table[i][j]);
        }
        self->sectionKeyCounts[i] = 0;
        self->sectionHashes[i] = 0;
    }
}

void ini_load(Ini* self, const char* filename) {

}

void ini_add_section(Ini* self, char* section) {
    ASSERT(self->sectionCount < INI_MAX_SECTIONS, "Max sections reached, consider increasing INI_MAX_SECTIONS");

    u64 hash = _ini_djb2(section);

    for (u32 i = 0; i < self->sectionCount; ++i) {
        ASSERT(self->sectionHashes[i] != hash,
            "Attempt to add existing section as key OR we have a hash collision.");
    }

    self->sectionHashes[self->sectionCount] = hash;
    ++self->sectionCount;
}

void ini_add_key(Ini* self, char* section, char* key, char* value) {
    ASSERT(!ini_exists(self, section, key), "Attempt to add key that already exists OR a hash collision in key names.");

    int sectionIndex = ini_section_index(self, section);

    u64 keyHash = _ini_djb2(key);
    int keyIndex = self->sectionKeyCounts[sectionIndex];
    self->table[sectionIndex][keyIndex].key = keyHash;
    self->table[sectionIndex][keyIndex].value = value;
    ++self->sectionKeyCounts[sectionIndex];
}

void ini_set_key(Ini* self, char* section, char* key, char* value) {
    int sectionIndex = ini_section_index(self, section);

    ASSERT(sectionIndex > -1, "Section does not exist.");

    int keyIndex = ini_index(self, section, key);

    ASSERT(keyIndex > -1, "Key does not exist.");

    // TODO: automatically handle freeing of previous value???
    self->table[sectionIndex][keyIndex].value = value;
}

int ini_section_index(Ini* self, char* section) {
    u64 hash = _ini_djb2(section);

    for (u32 i = 0; i < self->sectionCount; ++i) {
        if (self->sectionHashes[i] == hash) {
            return (int)i;
        }
    }

    return -1;
}

int ini_index(Ini* self, char* section, char* key) {
    int index = ini_section_index(self, section);

    ASSERT(index > -1, "Section does not exist.");

    u64 hash = _ini_djb2(key);

    for (u32 i = 0; i < self->sectionKeyCounts[index]; ++i) {
        IniTreeKvp kvp = self->table[index][i];

        if (hash == kvp.key) {
            return (int)i;
        }
    }

    return -1;
}

bool ini_section_exists(Ini* self, char* section) {
    return (ini_section_index(self, section) > -1);
}

bool ini_exists(Ini* self, char* section, char* key) {
    return (ini_index(self, section, key) > -1);
}

int ini_section_count(Ini* self) {
    return self->sectionCount;
}

int ini_keys_count(Ini* self, char* section) {
    int index = ini_section_index(self, section);

    ASSERT(index > -1, "Section does not exist.");

    return self->sectionKeyCounts[index];
}

int ini_get_int(Ini* self, char* section, char* key) {
    char* str = ini_get_string(self, section, key);

    return strtol(str, NULL, 0);
}

float ini_get_float(Ini* self, char* section, char* key) {
    char* str = ini_get_string(self, section, key);

    return strtof(str, NULL);
}

bool ini_get_bool(Ini* self, char* section, char* key) {
    char* str = ini_get_string(self, section, key);

    char c = str[0];

    if (c == 'y' || c == 'Y' || c == 't' || c == 'T' || c == '1') {
        return true;
    } else {
        return false;
    }
}

char* ini_get_string(Ini* self, char* section, char* key) {
    int sectionIndex = ini_section_index(self, section);

    ASSERT(sectionIndex > -1, "Section does not exist.");

    int keyIndex = ini_index(self, section, key);

    ASSERT(keyIndex > -1, "Key does not exist.");

    return self->table[sectionIndex][keyIndex].value;
}

// Collisions entirely plausible.
// According to https://programmers.stackexchange.com/questions/49550/which-hashing-algorithm-is-best-for-uniqueness-and-speed
// There's approximately a 4/216000 chance of one.  We will ASSERT if such a collision happens though in add_section.
u64 _ini_djb2(const char *key) {
    u64 hash = 5381;
    i32 c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c; //hash * 33 + c
    }
    return hash;
}

void _ini_strim(char* s) {
    _ini_striml(s);
    _ini_strimr(s);    
}

void _ini_striml(char* s) {
    int index = 0;

    while (_ini_isws(s[index])) {
        ++index;
    }

    memmove(s, &s[index], strlen(s) - index);
}

void _ini_strimr(char* s) {
    int len = (int)strlen(s);
    int index = len - 1;
    while (index >= 0) {
        if (_ini_isws(s[index])) {
            --index;
        } else {
            s[index + 1] = 0x0;
            break;
        }
    }
}

bool _ini_isws(char c) {
    return (c == 0x20 || c == 0x09);
}