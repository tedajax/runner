#include "application.h"
#include "tdjxlua.h"

int main(int argc, char* argv[]) {
    lua_State* L = lua_open();
    luaL_openlibs(L);
    luaL_dofile(L, "assets/scripts/test.lua");
    printf("%s\n", lua_tostring(L, -1));
    return app_run(argc, argv);
}
