#include "luasystem.h"

void lua_system_start_component(LuaSystem* self, LuaComponent* lua);

void lua_system_init(LuaSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_LUA);

    timer_init(&self->reloadTimer, 1.f, -1, lua_system_check_and_reload_void);

    REGISTER_SYSTEM_HANDLER(MESSAGE_ENTITY_REMOVED, lua_system_on_entity_removed);
}

void lua_system_start_component(LuaSystem* self, LuaComponent* lua) {
    if (lua->state == LUA_STATE_NEW) {
        lua_bind_call(&lua->callbackBinds[LUA_CALLBACK_START], lua->L);
        lua->state = LUA_STATE_RUNNING;
    }
}

void lua_system_start(LuaSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        LuaComponent* lua = (LuaComponent*)GET_SYSTEM_COMPONENT(i);

        REQUIRED_COMPONENTS(lua);

        //TODO: handle not having the function
        lua_system_start_component(self, lua);
    }
}

void lua_system_update(LuaSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    timer_tick(&self->reloadTimer, globals.time.delta, (void*)self);

    for (u32 i = 0; i < components->count; ++i) {
        LuaComponent* lua = (LuaComponent*)GET_SYSTEM_COMPONENT(i);

        REQUIRED_COMPONENTS(lua);

        if (lua->state == LUA_STATE_NEW) {
            lua_system_start_component(self, lua);
        }

        //TODO: handle not having the function
        lua_bind_call(&lua->callbackBinds[LUA_CALLBACK_UPDATE], lua->L, globals.time.delta);
    }
}

void lua_system_render(LuaSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        LuaComponent* lua = (LuaComponent*)GET_SYSTEM_COMPONENT(i);

        REQUIRED_COMPONENTS(lua);

        //TODO: handle not having the function
        lua_bind_call(&lua->callbackBinds[LUA_CALLBACK_RENDER], lua->L);
    }
}

void lua_system_check_and_reload_void(void* system) {
    lua_system_check_and_reload((LuaSystem*)system);
}

void lua_system_check_and_reload(LuaSystem* self) {
    GET_SYSTEM_COMPONENTS(self);
    for (u32 i = 0; i < components->count; ++i) {
        LuaComponent* lua = (LuaComponent*)GET_SYSTEM_COMPONENT(i);

        REQUIRED_COMPONENTS(lua);

        lua_component_check_and_reload(lua);
    }
}

void lua_system_on_entity_removed(AspectSystem* system, Entity entity, Message message) {

}