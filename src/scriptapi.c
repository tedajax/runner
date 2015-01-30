#include "scriptapi.h"
#include "gametime.h"

LUA_EXPORT void quit() {
    exit(0);
}

LUA_EXPORT int get_time_ms() {
    return (int)game_time_nano_to_milli(game_time_now());
}