#include "application.h"
#include "tdjxlua.h"

LUA_EXPORT void test_lua()
{
    printf("called from lua!\n");
}

LUA_EXPORT void quit()
{
    exit(0);
}

int main(int argc, char* argv[]) {
    lua_State* L = lua_open();
    luaL_openlibs(L);
    luaL_dofile(L, "assets/scripts/test.lua");
    return app_run(argc, argv);
}
