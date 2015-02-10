#include "animation.h"

void animation_frame_list_init(AnimationFrameList* self, const char* animationName, u32 count) {
    self->count = count;
    self->frames = CALLOC(self->count, AnimationFrame);
}

void animation_frame_list_free(AnimationFrameList* self) {
    free(self->frames);
}


void animation_init(Animation* self, Atlas* atlas, const char* name) {

}

void animation_free(Animation* self) {

}

void animation_update(Animation* self, f32 dt) {

}

void animation_pause(Animation* self) {

}

void animation_play(Animation* self, bool loop) {

}

void animation_reverse(Animation* self, bool loop) {

}

void animation_set_frame(Animation* self, u32 frame) {

}

SpriteFrame animation_get_frame(Animation* self) {

}