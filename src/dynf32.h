#ifndef RUNNER_DYNF32_H
#define RUNNER_DYNF32_H

#include "types.h"
#include "inline.h"
#include "ini.h"

// forward declarations
typedef struct tween_t Tween;
typedef struct tween_config_t TweenConfig;
typedef struct tween_manager_t TweenManager;

typedef enum dynamic_f32_type_e {
    DYN_F32_VALUE,
    DYN_F32_TWEEN,
} DynamicFloatType;

typedef struct dynamic_f32_t {
    f32 value;
    DynamicFloatType type;
    Tween* tween;
    TweenConfig* tweenConfig;
} dynf32;

void dynf32_zero(dynf32* self);
void dynf32_release(dynf32* self);
void dynf32_start_tween(dynf32* self, TweenManager* tweenManager);
void dynf32_restart(dynf32* self, TweenManager* tweenManager);
void dynf32_copy(const dynf32* source, dynf32* dest);
f32 dynf32_get(dynf32* self);
bool dynf32_set(dynf32* self, f32 value);

#endif