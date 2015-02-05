#ifndef RUNNER_LUA_COMPONENT_H
#define RUNNER_LUA_COMPONENT_H

#include "core.h"
#include "component.h"
#include "luabind.h"

#include <sys/types.h>
#include <sys/stat.h>

typedef enum lua_component_cb_e {
    LUA_CALLBACK_START,
    LUA_CALLBACK_UPDATE,
    LUA_CALLBACK_RENDER,
    LUA_CALLBACK_LAST,
} LuaComponentCallbacks;

typedef enum lua_component_state_e {
    LUA_STATE_NEW,
    LUA_STATE_RUNNING,
    LUA_STATE_PAUSED
} LuaComponentState;

typedef struct lua_file_t {
    char* path;
    time_t lastMTime;
} LuaFile;

typedef struct lua_component_t {
    Component super;
    LuaFile file;
    lua_State* L;
    LuaComponentState state;
    LuaBind callbackBinds[LUA_CALLBACK_LAST];
} LuaComponent;

LuaComponent* lua_component_new(Entity entity, const char* filename);
COMPONENT_DESERIALIZE(COMPONENT_LUA);
void lua_component_check_and_reload(LuaComponent* self);
void lua_component_free(LuaComponent* self);

#endif