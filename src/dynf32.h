#ifndef RUNNER_DYNF32_H
#define RUNNER_DYNF32_H

#include "types.h"
#include "tween.h"
#include "inline.h"
#include "ini.h"

typedef struct dynamic_f32_t {
	f32 value;
	Tween* tween;
} dynf32;

void dynf32_init(dynf32* self, f32 value, Tween* tween);
void dynf32_zero(dynf32* self);
f32 dynf32_get(dynf32* self);
bool dynf32_set(dynf32* self, f32 value);
dynf32 dynf32_config_at(TweenManager* tweenManager, Ini* config, const char* section, const char* key, u32 index);

inline dynf32 dynf32_config(TweenManager* tweenManager, Ini* config, const char* section, const char* key) { dynf32_config_at(tweenManager, config, section, key, 0); }

#endif