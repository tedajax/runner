#include "tween.h"
#include <math.h>

void tween_init(Tween* self, f32 start, f32 end, f32 duration, u32 loops, tween_func tweenFunc) {
    self->start = start;
    self->end = end;
    self->duration = duration;
    self->loops = loops;

    self->tweenFunc = tweenFunc;

    self->time = 0.f;
}

f32 tween_evaluate(Tween* self) {
    ASSERT(self->tweenFunc, "Attempt to evaluate tween with no tweening function.");

    return self->tweenFunc(self->time, self->start, self->end, self->duration);
}

f32 tween_linear(f32 t, f32 i, f32 f, f32 d) {
    return (f - i) * t / d + i;
}

f32 tween_ease_in_quad(f32 t, f32 i, f32 f, f32 d) {
    t /= d;
    return (f - i) * t * t + i;
}

f32 tween_ease_out_quad(f32 t, f32 i, f32 f, f32 d) {
    t /= d;
    return -(f - i) * (t - 2.f) + i;
}

f32 tween_ease_in_out_quad(f32 t, f32 i, f32 f, f32 d) {
    t /= (d / 2.f);
    if (t < 1.f) {
        return (f - i) / 2.f * t * t + i;
    }
    t -= 1.f;
    return -(f - i) / 2 * (t * (t - 2) - 1) + i;
}

f32 tween_ease_in_sin(f32 t, f32 i, f32 f, f32 d) {
    return -(f - i) * sinf(t / d * TWO_PI) + i;
}

f32 tween_ease_out_sin(f32 t, f32 i, f32 f, f32 d) {
    return (f - i) * sinf(t / d * TWO_PI) + i;
}

f32 tween_ease_in_out_sin(f32 t, f32 i, f32 f, f32 d) {
    return -(f - i) / 2.f * (cosf(PI * t / d) - 1.f) + i;
}

f32 tween_ease_in_expo(f32 t, f32 i, f32 f, f32 d) {
    return (f - i) * powf(2.f, 10.f * (t / d - 1.f)) + i;
}

f32 tween_ease_out_expo(f32 t, f32 i, f32 f, f32 d) {
    return (f - i) * (-powf(2.f, -10.f * t / d) + 1.f) + i;
}

f32 tween_ease_in_out_expo(f32 t, f32 i, f32 f, f32 d) {
    t /= (d / 2.f);
    if (t < 1.f) {
        return (f - i) / 2.f * powf(2.f, 10.f * (t - 1.f)) + i;
    }
    t -= 1.f;
    return (f - i) / 2.f * (-powf(2.f, -10.f * t) + 2.f) + i;
}

f32 tween_ease_in_circ(f32 t, f32 i, f32 f, f32 d) {
    t /= d;
    return -(f - i) * (sqrtf(1.f - t * t) - 1.f) + i;
}

f32 tween_ease_out_circ(f32 t, f32 i, f32 f, f32 d) {
    t /= d;
    t -= 1.f;
    return (f - i) * sqrtf(1.f - t * t) + i;
}

f32 tween_ease_in_out_circ(f32 t, f32 i, f32 f, f32 d) {
    t /= (d / 2.f);
    if (t < 1.f) {
        return -(f - i) / 2.f * (sqrtf(1.f - t * t) - 1.f) + i;
    }
    t -= 2.f;
    return (f - i) / 2.f * (sqrtf(1.f - t * t) + 1.f) + i;
}

f32 tween_bounce_in(f32 t, f32 i, f32 f, f32 d) {
    return (f - i) - tween_bounce_out(d - t, 0.f, f, d) + i;
}

f32 tween_bounce_out(f32 t, f32 i, f32 f, f32 d) {
    t /= d;
    if (t < (1.f / 2.75f)) {
        return (f - i) * (7.5625f * t * t) + i;
    } else if (t < (2.f / 2.75f)) {
        t -= (1.5f / 2.75f);
        return (f - i) * (7.5625f * t * t + .75f) + i;
    } else if (t < (2.5f / 2.75f)) {
        t -= (2.25f / 2.75f);
        return (f - i) * (7.5625f * t * t + .9375f) + i;
    } else {
        t -= (2.625f / 2.75f);
        return (f - i) * (7.5625f * t * t + .984375f) + i;
    }
}

f32 tween_bounce_in_out(f32 t, f32 i, f32 f, f32 d) {
    if (t < (d / 2.f)) {
        return tween_bounce_in(t * 2.f, 0.f, f, d) * 0.5f + i;
    } else {
        return tween_bounce_out(t * 2.f - d, 0.f, f, d) * 0.5f + (f - i) * 0.5f + i;
    }
}

f32 tween_sin_wave(f32 t, f32 i, f32 f, f32 d) {
    t /= d;
    return sinf(t * TWO_PI) * ((f - i) / 2.f) + i + ((f - i) / 2.f);
}

f32 tween_cos_wave(f32 t, f32 i, f32 f, f32 d) {
    t /= d;
    return cosf(t * TWO_PI) * ((f - i) / 2.f) + i + ((f - i) / 2.f);
}

f32 tween_parabolic(f32 t, f32 i, f32 f, f32 d) {
    return ((-4.f * (i + (f - i))) / (d * d)) * t * (t - d);
}