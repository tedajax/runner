#include "scriptapi.h"
#include "gametime.h"
#include "primitives.h"
#include "globals.h"

LUA_EXPORT void quit() {
    exit(0);
}

LUA_EXPORT int get_time_ms() {
    return (int)game_time_nano_to_milli(game_time_now());
}

LUA_EXPORT void draw_rectangle(Vec2 center, f32 width, f32 height) {
    draw_rectangle_rgba(center, width, height, 255, 255, 255, 255);
}

LUA_EXPORT void draw_rectangle_rgba(Vec2 center, f32 width, f32 height, u8 r, u8 g, u8 b, u8 a) {
    Rect rect;
    rect.position.x = center.x - width / 2.f;
    rect.position.y = center.y - height / 2.f;
    rect.width = width;
    rect.height = height;
    prim_box_rgba(globals.renderer, &rect, r, g, b, a);
}