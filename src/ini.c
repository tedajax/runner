#include "ini.h"

#include <string.h>
#include <ctype.h>

typedef enum ini_line_type_e {
    INI_LINE_IGNORE,
    INI_LINE_ERROR,
    INI_LINE_SECTION,
    INI_LINE_KVP,
} IniLineType;

// Indices in string where string exists
// start is at the character index that begins the sub string
// end is the character index that is AFTER the last character in the sub string
typedef struct ini_sub_string_t {
    u32 start;
    u32 end;
} IniSubString;

typedef struct ini_kvp_sub_strings_t {
    IniSubString key;
    IniSubString value;
} IniKvpSubStrings;

typedef union ini_line_value_t {
    IniKvpSubStrings keyValue;
    IniSubString section;
} IniLineValue;

typedef struct ini_line_result_t {
    IniLineType type;
    IniLineValue value;
} IniLineResult;

typedef struct ini_lines_t {
    char** lines;
    u32 count;
} IniLines;

// "private" utility functions
u64 _ini_djb2(const char *key);
void _ini_strim(char* s);
void _ini_striml(char* s);
void _ini_strimr(char* s);
void _ini_print_substr(char* s, u32 start, u32 end);
bool _ini_isws(char c);
IniLines _ini_split_lines(char* data);
void _ini_free_lines(IniLines* self);
char* _ini_load_file(const char* filename);
IniLineResult _ini_parse_line(char* line);

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
    ini_init(self);

    char* data = _ini_load_file(filename);
    IniLines lines = _ini_split_lines(data);

    char currentSection[256];

    for (u32 i = 0; i < lines.count; ++i) {
        IniLineResult line = _ini_parse_line(lines.lines[i]);
        
        if (line.type == INI_LINE_SECTION) {
            u32 start = line.value.section.start;
            u32 end = line.value.section.end;
            
            char* sectionName = calloc(end - start + 1, sizeof(char));
            strncpy(sectionName, &lines.lines[i][start], end - start);
            strncpy(&currentSection[0], sectionName, 256);
            ini_add_section(self, sectionName);
            free(sectionName);
        } else if (line.type == INI_LINE_KVP) {
            u32 keystart = line.value.keyValue.key.start;
            u32 keyend = line.value.keyValue.key.end;

            u32 valuestart = line.value.keyValue.value.start;
            u32 valueend = line.value.keyValue.value.end;

            char* keyName = calloc(keyend - keystart + 1, sizeof(char));
            char* valueName = calloc(valueend - valuestart + 1, sizeof(char));

            strncpy(keyName, &lines.lines[i][keystart], keyend - keystart);
            strncpy(valueName, &lines.lines[i][valuestart], valueend - valuestart);

            ini_add_key(self, &currentSection[0], keyName, valueName);

            free(keyName);
            // dont free the value since it's actually stored in the table
        } else if (line.type == INI_LINE_ERROR) {
            log_error_format("INI", "Error parsing INI file \'%s\' on line %u.", filename, i + 1);
            return;
        }
    }

    free(data);
    _ini_free_lines(&lines);
}

void ini_free(Ini* self) {
    self->sectionCount = 0;
    for (u32 i = 0; i < INI_MAX_SECTIONS; ++i) {
        for (u32 j = 0; j < INI_MAX_KEYS_PER_SECTION; ++j) {
            if (self->table[i][j].key > 0) {
                free(self->table[i][j].value);
            }
        }
        self->sectionKeyCounts[i] = 0;
        self->sectionHashes[i] = 0;
    }
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

    ASSERT(sectionIndex > -1, "Invalid section.");

    int keyIndex = ini_index(self, section, key);

    ASSERT(keyIndex > -1, "Key not found in section.");

    return self->table[sectionIndex][keyIndex].value;
}

int ini_try_get_int(Ini* self, char* section, char* key, int defaultVal) {
    char* str = ini_try_get_string(self, section, key, NULL);

    if (!str) {
        return defaultVal;
    }

    return strtol(str, NULL, 0);
}

float ini_try_get_float(Ini* self, char* section, char* key, float defaultVal) {
    char* str = ini_try_get_string(self, section, key, NULL);

    if (!str) {
        return defaultVal;
    }

    return strtof(str, NULL);
}

bool ini_try_get_bool(Ini* self, char* section, char* key, bool defaultVal) {
    char* str = ini_try_get_string(self, section, key, NULL);

    if (!str) {
        return defaultVal;
    }

    char c = str[0];

    if (c == 'y' || c == 'Y' || c == 't' || c == 'T' || c == '1') {
        return true;
    } else {
        return false;
    }
}

char* ini_try_get_string(Ini* self, char* section, char* key, char* defaultVal) {
    int sectionIndex = ini_section_index(self, section);

    if (sectionIndex == -1) {
        return defaultVal;
    }

    int keyIndex = ini_index(self, section, key);

    if (keyIndex == -1) {
        return defaultVal;
    }

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

void _ini_print_substr(char* s, u32 start, u32 end) {
    for (u32 i = start; i < end; ++i) {
        printf("%c", s[i]);
    }
}

bool _ini_isws(char c) {
    return (c == 0x20 || c == 0x09 || c == 0xD);
}

IniLines _ini_split_lines(char* data) {
    IniLines dest;

    u32 capacity = 64;
    // Allocate initial block of lines, we will realloc if we need more
    dest.lines = (char**)calloc(capacity, sizeof(char*));
    dest.count = 0;

    size_t len = strlen(data);  
    size_t index = 0;
    for (size_t i = 0; i < len; ++i) {
        if (data[i] == '\n') {
            dest.lines[dest.count] = (char*)calloc(i - index + 1, sizeof(char));
            strncpy(dest.lines[dest.count], &data[index], i - index);
            dest.lines[dest.count][i - index] = '\0';
            ++dest.count;

            if (dest.count >= capacity) {
                capacity <<= 1;
                dest.lines = (char**)realloc(dest.lines, sizeof(char*) * capacity);
            }

            index = i + 1;
        }
    }

    if (len - index > 0) {
        dest.lines[dest.count] = (char*)calloc(len - index + 1, sizeof(char));
        strncpy(dest.lines[dest.count], &data[index], len - index);
        dest.lines[dest.count][len - index] = '\0';
        ++dest.count;
    }

    return dest;
}

void _ini_free_lines(IniLines* self) {
    for (u32 i = 0; i < self->count; ++i) {
        free(self->lines[i]);
    }
    free(self->lines);
}

char* _ini_load_file(const char* filename) {
    long fileSize;
    FILE* file = fopen(filename, "rb");
    
    ASSERT(file, "Failed to open file.");

    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    char* dest = (char*)calloc(fileSize + 1, sizeof(char));
    fread(dest, sizeof(char), fileSize, file);

    fclose(file);

    return dest;
}

IniLineResult _ini_parse_line(char* line) {
    IniLineResult result;

    _ini_strim(line);

    IniLineType type = INI_LINE_IGNORE;

    u32 index = 0;
    u32 len = (u32)strlen(line);
    char c = line[index];
    bool done = false;

    u32 sectionStart = 0;
    u32 sectionEnd = 0;

    u32 keyStart = 0;
    u32 keyEnd = 0;

    u32 valueStart = 0;
    u32 valueEnd = 0;
    
    while (c && !done) {
        switch (type) {
            case INI_LINE_IGNORE:
                if (c == '[') {
                    type = INI_LINE_SECTION;
                    sectionStart = index + 1;
                } else if (c == ';' || c == '#') {
                    done = true;
                } else if (isalpha(c)) {
                    type = INI_LINE_KVP;
                    keyStart = index;
                }
                break;
            
            case INI_LINE_SECTION:
                if (_ini_isws(c)) {
                    goto ini_parse_error;
                } else if (c == ']') {
                    sectionEnd = index;
                    done = true;
                }
                break;

            case INI_LINE_KVP:
                if (keyEnd == 0) {
                    if (isalnum(c)) {
                        // nothing
                    } else if (_ini_isws(c) || c == '=') {
                        keyEnd = index;
                    } else {
                        goto ini_parse_error;
                    }
                } else if (valueEnd == 0) {
                    if (valueStart == 0) {
                        if (isalnum(c) || c == '-') {
                            valueStart = index;
                        }
                    } else {
                        if (_ini_isws(c) || index == len - 1 || c == '#' || c == ';') {
                            valueEnd = index + 1;
                        }
                    }
                }
                break;

            default:
                goto ini_parse_error;
        }

        c = line[++index];
    }

    if (type == INI_LINE_SECTION) {
        if (sectionEnd <= sectionStart) {
            goto ini_parse_error;
        }

        result.value.section.start = sectionStart;
        result.value.section.end = sectionEnd;
    } else if (type == INI_LINE_KVP) {
        if (keyEnd <= keyStart) {
            goto ini_parse_error;
        }

        if (valueEnd <= valueStart) {
            goto ini_parse_error;
        }

        if (valueStart <= keyEnd) {
            goto ini_parse_error;
        }

        result.value.keyValue.key.start = keyStart;
        result.value.keyValue.key.end = keyEnd;

        result.value.keyValue.value.start = valueStart;
        result.value.keyValue.value.end = valueEnd;
    }

    result.type = type;
    return result;

ini_parse_error:
    result.type = INI_LINE_ERROR;
    return result;
}