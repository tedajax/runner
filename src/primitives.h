#ifndef RUNNER_PRIMITIVES_H
#define RUNNER_PRIMITIVES_H

/*
 * Functions for rendering primitives (pixels, lines, squares, circles, etc)
 * Uses SDL2 for the renderering.
 * Built because the only reason SDL2_gfx is a dependency is for these rendering functions but SDL2_gfx has some deficiencies:
 *      - All the functions in SDL2_gfx use Sint16 for coordinates but SDL2 uses ints so things wrap around early.
 *      - SDL2_gfx is not a real official part of SDL2 and so getting libs for it is a bit harder.
 *      - The only thing I'm using gfx for is the primitives drawing stuff and SDL2 has plenty of functions to handle it all already.
 *      - I can wrap the SDL2 calls more to my liking with my own thing.
 *      - Some unnecessary primitive stuff like antialiased lines and stuff like that.  I don't need those this is just for debug output.
*/

#include <SDL2/SDL.h>
#include "types.h"
#include "algebra.h"

int prim_pixel(SDL_Renderer* renderer, Vec2* position);
int prim_pixel_color(SDL_Renderer* renderer, Vec2* position, u32 color);
int prim_pixel_rgba(SDL_Renderer* renderer, Vec2* position, u8 r, u8 g, u8 b, u8 a);

int prim_line(SDL_Renderer* renderer, Vec2* point1, Vec2* point2);
int prim_line_color(SDL_Renderer* renderer, Vec2* point1, Vec2* point2, u32 color);
int prim_line_rgba(SDL_Renderer* renderer, Vec2* point1, Vec2* point2, u8 r, u8 g, u8 b, u8 a);

int prim_rect(SDL_Renderer* renderer, Rect* rect);
int prim_rect_color(SDL_Renderer* renderer, Rect* rect, u32 color);
int prim_rect_rgba(SDL_Renderer* renderer, Rect* rect, u8 r, u8 g, u8 b, u8 a);

int prim_box(SDL_Renderer* renderer, Rect* rect);
int prim_box_color(SDL_Renderer* renderer, Rect* rect, u32 color);
int prim_box_rgba(SDL_Renderer* renderer, Rect* rect, u8 r, u8 g, u8 b, u8 a);

int prim_circle(SDL_Renderer* renderer, Circle* circle, u32 segments);
int prim_circle_color(SDL_Renderer* renderer, Circle* circle, u32 segments, u32 color);
int prim_circle_rgba(SDL_Renderer* renderer, Circle* circle, u32 segments, u8 r, u8 g, u8 b, u8 a);

#endif