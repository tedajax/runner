#ifndef RUNNER_ANIMATION_H
#define RUNNER_ANIAMTION_H

#include "core.h"
#include "atlas.h"

typedef struct animation_frame_t {
    SpriteFrame frame;
    f32 frameInterval;
} AnimationFrame;

typedef struct animation_frame_list_t {
    AnimationFrame* frames;
    u32 count;
} AnimationFrameList;

typedef enum animation_state_e {
    ANIMATION_PAUSED,
    ANIMATION_FORWARD,
    ANIMATION_BACKWARD,
} AnimationState;

typedef struct animation_t {
    AnimationFrameList frames;
    AnimationState state;
    bool looping;
    u32 currentFrame;
    f32 frameTimer;
} Animation;

void animation_frame_list_init(AnimationFrameList* self, const char* animationName, u32 count);
void animation_frame_list_free(AnimationFrameList* self);

void animation_init(Animation* self, Atlas* atlas, const char* name);
void animation_free(Animation* self);
void animation_update(Animation* self, f32 dt);
void animation_pause(Animation* self);
void animation_play(Animation* self, bool loop);
void animation_reverse(Animation* self, bool loop);
void animation_set_frame(Animation* self, u32 frame);
SpriteFrame animation_get_frame(Animation* self);

#endif