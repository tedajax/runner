#ifndef RUNNER_SCRIPT_API_H
#define RUNNER_SCRIPT_API_H
#include "tdjxlua.h"

LUA_EXPORT void quit();
void Sleep(int ms);
LUA_EXPORT int get_time_ms();

#endif