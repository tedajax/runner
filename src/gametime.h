#ifndef RUNNER_TIME_H
#define RUNNER_TIME_H

#include <SDL2/SDL.h>

#include "types.h"
#include "debug.h"
#include "log.h"

extern const u32 SECONDS_TO_NANOSECONDS;

typedef void (*time_cb)();

typedef struct game_time_t {
    u64 last_frame_ticks;
    u64 last_frame_ns;
    u64 since_start_ns;
    u64 delta_ns;
    u32 seconds_timer_ns;
    u32 frames_this_second;
    u32 fps;
    u64 per_frame_ms;
    f32 delta;
    f32 timescale;
    time_cb on_second;
} GameTime;

//static u64 game_time_ticker;

void game_time_initialize(GameTime* self);
void game_time_update(GameTime* self);
u64 game_time_now();
u64 game_time_nano_to_micro(u64 ns);
u64 game_time_nano_to_milli(u64 ns);

u64 game_time_tick();
u64 game_time_tock();

#endif