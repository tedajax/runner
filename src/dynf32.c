#include "dynf32.h"

void dynf32_init(dynf32* self, f32 value, Tween* tween) {
	self->value = value;
	self->tween = tween;
}

void dynf32_zero(dynf32* self) {
	self->value = 0.f;
	self->tween = NULL;
}

f32 dynf32_get(dynf32* self) {
	if (self->tween) {
		return tween_evaluate(self->tween);
	} else {
		return self->value;
	}
}

bool dynf32_set(dynf32* self, f32 value) {
	if (self->tween) {
		return false;
	} else {
		self->value = value;
		return true;
	}
}

dynf32 dynf32_config_at(TweenManager* tweenManager, Ini* config, const char* section, const char* key, u32 index) {
	dynf32 result;
	dynf32_zero(&result);

	char* str = ini_get_string_at(config, section, key, index);

	char* endptr;
	result.value = strtof(str, &endptr);

	if (*endptr == '\0') {
		return result;
	}

	result.tween = tween_manager_create_config(tweenManager, config, str);
	return result;
}