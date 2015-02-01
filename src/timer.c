#include "timer.h"

void timer_init(Timer* self, f32 interval, i32 count, timer_cb callback) {
    self->interval = interval;
    self->count = count;
    self->timer = 0.f;
    self->remaining = self->count;
    self->callback = callback;
}

void timer_tick(Timer* self, f32 dt, void* payload) {
    if (self->remaining == 0) {
        return;
    }

    self->timer += dt;
    if (self->timer >= self->interval) {
        self->timer = 0.f;
        if (self->remaining > 0) {
            --self->remaining;
        }
        self->callback(payload);
    }
}