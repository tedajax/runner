#ifndef RUNNER_LUA_COMPONENT_H
#define RUNNER_LUA_COMPONENT_H

#include "core.h"
#include "component.h"
#include "luabind.h"

typedef enum lua_component_cb_e {
    LUA_CALLBACK_START,
    LUA_CALLBACK_UPDATE,
    LUA_CALLBACK_RENDER,
    LUA_CALLBACK_LAST,
} LuaComponentCallbacks;

typedef struct lua_component_t {
    Component super;
    lua_State* L;
    LuaBind callbackBinds[LUA_CALLBACK_LAST];
} LuaComponent;

LuaComponent* lua_component_new(const char* filename)

#endif