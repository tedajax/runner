#ifndef RUNNER_LUA_SYSTEM_H
#define RUNNER_LUA_SYSTEM_H

#include "aspectsystem.h"
#include "component.h"
#include "luacomponent.h"
#include "timer.h"

typedef struct lua_system_t {
    AspectSystem super;
    Timer reloadTimer;
} LuaSystem;

void lua_system_init(LuaSystem* self, EntityManager* entityManager);

void lua_system_start(LuaSystem* self);
void lua_system_update(LuaSystem* self);
void lua_system_render(LuaSystem* self);

void lua_system_check_and_reload_void(void* system);
void lua_system_check_and_reload(LuaSystem* self);

#endif