#ifndef RUNNER_LUA_BIND_H
#define RUNNER_LUA_BIND_H

#include <string.h>
#include <stdarg.h>

#include "tdjxlua.h"

#include "types.h"
#include "debug.h"

typedef enum lua_bind_arg_types_e {
    LUA_ARG_BOOLEAN,
    LUA_ARG_INTEGER,
    LUA_ARG_FLOAT,
    LUA_ARG_STRING,
} LuaBindArgTypes;

typedef struct lua_bind_t {
    char* functionName;
    int argc;
    LuaBindArgTypes* argt;
} LuaBind;

void lua_bind_init(LuaBind* self, const char* function, int argc, ...);
void lua_bind_call(LuaBind* self, lua_State* L, ...);
void lua_bind_callv(LuaBind* self, lua_State* L, va_list argv);
void lua_bind_free(LuaBind* self);

#define LUA_ERROR(L) \
    fprintf(stderr, "Lua error -- %s:%d: %s\n", __FILE__, __LINE__, lua_tostring(L, -1));

#endif