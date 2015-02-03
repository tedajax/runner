#include "gametime.h"

const u32 SECONDS_TO_NANOSECONDS = 1000000000;
static u64 game_time_ticker = 0;

void game_time_initialize(GameTime* self) {
    self->last_frame_ticks = 0;
    self->last_frame_ns = 0;
    self->since_start_ns = 0;
    self->delta_ns = 0;
    self->seconds_timer_ns = 0;
    self->frames_this_second = 0;
    self->fps = 0;
    self->per_frame_ms = 0;
    self->delta = 0.f;
    self->timescale = 1.f;
    self->on_second = NULL;
}

void game_time_update(GameTime* self) {
    {
        u64 ticks = SDL_GetPerformanceCounter();
        u64 frequency = SDL_GetPerformanceFrequency();

        if (self->last_frame_ticks == 0) {
            self->last_frame_ticks = ticks;
        }

        u64 diff = ticks - self->last_frame_ticks;
        diff *= SECONDS_TO_NANOSECONDS;
        diff /= frequency;

        self->delta_ns = diff;
        self->last_frame_ticks = ticks;
    }

    self->last_frame_ns = self->since_start_ns;
    self->since_start_ns += self->delta_ns;

    self->per_frame_ms = game_time_nano_to_milli(self->delta_ns);

    self->seconds_timer_ns += (u32)self->delta_ns;
    if (self->seconds_timer_ns >= (1 * SECONDS_TO_NANOSECONDS)) {
        self->seconds_timer_ns = 0;
        self->fps = self->frames_this_second;
        self->frames_this_second = 0;
        if (self->on_second) {
            self->on_second();
        }
    } else {
        ++self->frames_this_second;
    }

    self->delta_no_scale = ((f32)self->since_start_ns - (f32)self->last_frame_ns) / SECONDS_TO_NANOSECONDS;
    self->delta = self->delta_no_scale * self->timescale;
}

u64 game_time_now() {
    
    u64 ticks = SDL_GetPerformanceCounter();
    u64 frequency = SDL_GetPerformanceFrequency();

    u64 result = ticks * SECONDS_TO_NANOSECONDS;
    result /= frequency;

    return result;
}

u64 game_time_nano_to_micro(u64 ns) {
    return ns / 1000;
}

u64 game_time_nano_to_milli(u64 ns) {
    return ns / 1000000;
}

u64 game_time_tick() {
    game_time_ticker = game_time_now();
    return game_time_ticker;
}

u64 game_time_tock() {
    return game_time_now() - game_time_ticker;
}
