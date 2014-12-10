#ifndef RUNNER_TWEEN_H
#define RUNNER_TWEEN_H

#include "core.h"

// Tween function take the form
// f32 tween_function(f32 t, f32 i, f32 f, f32 d)
// where:
//      t - current time of the tween in seconds.
//      i - initial value.
//      f - desired final value.
//      d - total duration of the tween in seconds.
//
typedef f32(*tween_func)(f32, f32, f32, f32);

f32 tween_linear(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_in_quad(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_out_quad(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_in_out_quad(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_in_sin(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_out_sin(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_in_out_sin(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_in_expo(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_out_expo(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_in_out_expo(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_in_circ(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_out_circ(f32 t, f32 i, f32 f, f32 d);
f32 tween_ease_in_out_circ(f32 t, f32 i, f32 f, f32 d);
f32 tween_bounce_in(f32 t, f32 i, f32 f, f32 d);
f32 tween_bounce_out(f32 t, f32 i, f32 f, f32 d);
f32 tween_bounce_in_out(f32 t, f32 i, f32 f, f32 d);
f32 tween_sin_wave(f32 t, f32 i, f32 f, f32 d);
f32 tween_cos_wave(f32 t, f32 i, f32 f, f32 d);
f32 tween_parabolic(f32 t, f32 i, f32 f, f32 d);

enum tween_loop_e {
    TWEEN_LOOP_NONE = 0,
    TWEEN_LOOP_INFINITE = 0xFFFFFFFF,
};

typedef struct tween_t {
    f32 start; // value at the start
    f32 end; // value at the end
    f32 duration; // duration in seconds
    u32 loops; // number of times to loop the tween, 0 = don't loop, 0xFFFFFFFF = loop infinitely

    f32 timescale; // multiply rate of time by this value, 1 - normal, 2 - double speed, 0.5 - half speed, etc...

    tween_func tweenFunc; // which tweenFunction to use.

    f32 time;
    u32 loopsLeft;        // how many loops are left

    bool enabled;
} Tween;

void tween_init(Tween* self, f32 start, f32 end, f32 duration, u32 loops, tween_func tweenFunc);
void tween_zero(Tween* self);
f32 tween_evaluate(Tween* self);
void tween_update(Tween* self, f32 dt);
void tween_play(Tween* self);
void tween_pause(Tween* self);
void tween_stop(Tween* self);
void tween_reset(Tween* self);

typedef struct tween_manager_T {
    Tween* tweens;
    u32* freeIndices;
    u32 capacity;
    u32 freeHead;
} TweenManager;

void tween_manager_init(TweenManager* self, u32 capacity);
void tween_manager_update(TweenManager* self, f32 dt);
Tween* tween_manager_create(TweenManager* self, f32 start, f32 end, f32 duration, u32 loops, tween_func tweenFunc);
void tween_manager_remove(TweenManager* self, Tween* tween);

#endif