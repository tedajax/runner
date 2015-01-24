#ifndef RUNNER_AI_CONTROLLER_H
#define RUNNER_AI_CONTROLLER_H

#include "core.h"
#include "bulletsource.h"

#define AI_MAX_STATES 16

typedef u32(*state_transition_f)(Component*);

typedef struct ai_controller_t {
    Entity target;
    u32 state;
    state_transition_f transitionFuncs[AI_MAX_STATES];
} AIController;

void ai_controller_init(AIController* self);
void ai_controller_set_state_transition(AIController* self, u32 state, state_transition_f transitionFunc);
bool ai_controller_update_state(AIController* self, Component* component);

#endif