#ifndef TDJX_LUA_H
#define TDJX_LUA_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <luajit.h>

#ifdef _MSC_VER
    #define LUA_EXPORT __declspec(dllexport)
#else
    #define LUA_EXPORT
#endif

#endif