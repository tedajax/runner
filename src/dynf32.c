#include "dynf32.h"

void dynf32_init(dynf32* self, f32 value, Tween* tween) {
	self->value = value;
	self->tween = tween;

    if (self->tween) {
        self->type = DYN_F32_TWEEN;
        tween_config_init_from_tween(&self->tweenConfig, tween);
    } else {
        self->type = DYN_F32_VALUE;
    }
}

void dynf32_zero(dynf32* self) {
	self->value = 0.f;
	self->tween = NULL;
}

void dynf32_release(dynf32* self) {
    if (self->type == DYN_F32_TWEEN) {
        tween_release(self->tween);
    }
}

void dynf32_start_tween(dynf32* self, TweenManager* tweenManager) {
    if (self->type == DYN_F32_TWEEN) {
        ASSERT(self->tween == NULL, "Tween has already been initialized.");
        
        self->tween = tween_manager_create(tweenManager, &self->tweenConfig);
        tween_play(self->tween);
    }
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

dynf32 dynf32_config_at(Ini* config, const char* section, const char* key, u32 index) {
	dynf32 result;
	dynf32_zero(&result);

	char* str = ini_get_string_at(config, section, key, index);

	char* endptr;
	result.value = strtof(str, &endptr);

	if (*endptr == '\0') {
        result.type = DYN_F32_VALUE;
		return result;
	}

    tween_config_init(&result.tweenConfig, config, str);
    result.tween = NULL;
    result.type = DYN_F32_TWEEN;
	return result;
}