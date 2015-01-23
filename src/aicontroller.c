#include "aicontroller.h"

void ai_controller_init(AIController* self) {
    self->target = 0;
    self->state = 0;
    for (u32 i = 0; i < AI_MAX_STATES; ++i) {
        self->transitionFuncs[i] = NULL;
    }
}

void ai_controller_set_state_transition(AIController* self, u32 state, state_transition_f transitionFunc) {
    ASSERT(state < AI_MAX_STATES, "State out of range.");

    self->transitionFuncs[state] = transitionFunc;
}

bool ai_controller_update_state(AIController* self, Component* component) {
    if (self->transitionFuncs[self->state]) {
        u32 newState = self->transitionFuncs[self->state](component);
        if (newState != self->state) {
            self->state = newState;
            return true;
        }
    }

    return false;
}