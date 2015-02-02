#ifndef RUNNER_SCRIPT_API_H
#define RUNNER_SCRIPT_API_H

#include "tdjxlua.h"
#include "types.h"
#include "algebra.h"

#if 0
typedef signed char         i8;
typedef signed short        i16;
typedef signed int          i32;
typedef signed long long    i64;

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

typedef float       f32;
typedef double      f64;

typedef struct vec_2_t {
    f32 x, y;
} Vec2;
#endif

LUA_EXPORT void quit();
void Sleep(int ms);
LUA_EXPORT int get_time_ms();

LUA_EXPORT void draw_rectangle(Vec2 center, f32 width, f32 height);
LUA_EXPORT void draw_rectangle_rgba(Vec2 center, f32 width, f32 height, u8 r, u8 g, u8 b, u8 a);

#endif