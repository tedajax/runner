#include "application.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

int main(int argc, char* argv[]) {
    lua_State *L;
    L = luaL_newstate();

    luaL_openlibs(L);

    lua_close(L);

    return app_run(argc, argv);
}
