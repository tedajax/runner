#include "primitives.h"

int prim_pixel(SDL_Renderer* renderer, Vec2* position) {
    return SDL_RenderDrawPoint(renderer, (int)position->x, (int)position->y);
}

int prim_pixel_color(SDL_Renderer* renderer, Vec2* position, u32 color) {
    Uint8* c = (Uint8*)&color;
    return prim_pixel_rgba(renderer, position, c[0], c[1], c[2], c[3]);
}

int prim_pixel_rgba(SDL_Renderer* renderer, Vec2* position, u8 r, u8 g, u8 b, u8 a) {
    int result = 0;
    result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
    result |= prim_pixel(renderer, position);
    return result;
}

int prim_line(SDL_Renderer* renderer, Vec2* point1, Vec2* point2) {
    return SDL_RenderDrawLine(renderer, (int)point1->x, (int)point1->y, (int)point2->x, (int)point2->y);
}

int prim_line_color(SDL_Renderer* renderer, Vec2* point1, Vec2* point2, u32 color) {
    Uint8* c = (Uint8*)&color;
    return prim_line_rgba(renderer, point1, point2, c[0], c[1], c[2], c[3]);
}

int prim_line_rgba(SDL_Renderer* renderer, Vec2* point1, Vec2* point2, u8 r, u8 g, u8 b, u8 a) {
    int result = 0;
    result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
    result |= prim_line(renderer, point1, point2);
    return result;
}

int prim_rect(SDL_Renderer* renderer, Rect* rect) {
    SDL_Rect r;
    rect_to_sdl_rect(rect, &r);
    return SDL_RenderDrawRect(renderer, &r);
}

int prim_rect_color(SDL_Renderer* renderer, Rect* rect, u32 color) {
    Uint8* c = (Uint8*)&color;
    return prim_rect_rgba(renderer, rect, c[0], c[1], c[2], c[3]);
}

int prim_rect_rgba(SDL_Renderer* renderer, Rect* rect, u8 r, u8 g, u8 b, u8 a) {
    int result = 0;
    result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
    result |= prim_rect(renderer, rect);
    return result;
}

int prim_box(SDL_Renderer* renderer, Rect* rect) {
    SDL_Rect r;
    rect_to_sdl_rect(rect, &r);
    return SDL_RenderFillRect(renderer, &r);
}

int prim_box_color(SDL_Renderer* renderer, Rect* rect, u32 color) {
    Uint8* c = (Uint8*)&color;
    return prim_box_rgba(renderer, rect, c[0], c[1], c[2], c[3]);
}

int prim_box_rgba(SDL_Renderer* renderer, Rect* rect, u8 r, u8 g, u8 b, u8 a) {
    int result = 0;
    result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
    result |= prim_box(renderer, rect);
    return result;
}

int prim_circle(SDL_Renderer* renderer, Circle* circle, u32 segments) {
    int result = 0;

    f32 degPerSegment = 360.f / segments;

    f32 x0 = circle->position.x + cosf(0.f) * circle->radius;
    f32 y0 = circle->position.y + sinf(0.f) * circle->radius;

    for (u32 s = 1; s <= segments; ++s) {
        f32 a = degPerSegment * DEG_TO_RAD * (f32)s;
        
        f32 x1 = circle->position.x + cosf(a) * circle->radius;
        f32 y1 = circle->position.y + sinf(a) * circle->radius;

        result |= SDL_RenderDrawLine(renderer, (int)x0, (int)y0, (int)x1, (int)y1);

        x0 = x1;
        y0 = y1;
    }

    return result;
}

int prim_circle_color(SDL_Renderer* renderer, Circle* circle, u32 segments, u32 color) {
    Uint8* c = (Uint8*)&color;
    return prim_circle_rgba(renderer, circle, segments, c[0], c[1], c[2], c[3]);
}

int prim_circle_rgba(SDL_Renderer* renderer, Circle* circle, u32 segments, u8 r, u8 g, u8 b, u8 a) {
    int result = 0;
    result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
    result |= prim_circle(renderer, circle, segments);
    return result;
}
