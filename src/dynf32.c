#include "dynf32.h"
#include "tween.h"

void dynf32_zero(dynf32* self) {
    self->value = 0.f;
    self->tween = NULL;
    self->tweenConfig = NULL;
}

void dynf32_release(dynf32* self) {
    if (self->type == DYN_F32_TWEEN) {
        tween_release(self->tween);
    }
}

void dynf32_start_tween(dynf32* self, TweenManager* tweenManager) {
    if (self->type == DYN_F32_TWEEN) {
        ASSERT(self->tween == NULL, "Tween has already been initialized.");
        
        self->tween = tween_manager_create(tweenManager, self->tweenConfig);
        tween_play(self->tween);
    }
}

void dynf32_restart(dynf32* self, TweenManager* tweenManager) {
    if (self->type != DYN_F32_TWEEN) {
        return;
    }

    if (self->tween != NULL) {
        dynf32_release(self);
    }

    dynf32_start_tween(self, tweenManager);
}

void dynf32_copy(const dynf32* source, dynf32* dest) {
    dest->type = source->type;
    dest->tweenConfig = source->tweenConfig;
    dest->value = source->value;
    dest->tween = NULL;
}

f32 dynf32_get(dynf32* self) {
    switch (self->type) {
        default:
        case DYN_F32_VALUE:
            return self->value;

        case DYN_F32_TWEEN:
            return tween_evaluate(self->tween);
    }
}

bool dynf32_set(dynf32* self, f32 value) {
    if (self->type == DYN_F32_VALUE) {
        self->value = value;
        return true;
    }

    return false;
}
