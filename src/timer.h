#ifndef RUNNER_TIMER_H
#define RUNNER_TIMER_H

#include "types.h"
#include "gametime.h"
#include "debug.h"

typedef void(*timer_cb)(void*);

typedef struct timer_t {
    f32 interval;
    i32 count;
    f32 timer;
    i32 remaining;
    timer_cb callback;
} Timer;

void timer_init(Timer* self, f32 interval, i32 count, timer_cb callback);
void timer_tick(Timer* self, f32 dt, void* payload);

#endif