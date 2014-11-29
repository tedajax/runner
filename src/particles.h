#ifndef RUNNER_PARTICLES_H
#define RUNNER_PARTICLES_H

#include "core.h"

#include <SDL2/SDL.h>

typedef struct particle_t {
    Vec2 position;
    Vec2 direction;
    f32 speed;
    f32 lifetime;
    f32 alpha;
    SDL_Texture* texture;
} Particle;

typedef struct particle_emitter_t {
    Particle* particles;
    u32 capacity;
    SDL_Texture* texture;
    //bool simulateLocally; // true to update particles in relation to emitter space, false to simulate them in world space
    f32 emitTimer;
    f32 emitInterval;
    u32 particlesPerEmit; // TODO: Support ranges (probably a feature of the config structure)
} ParticleEmitter;

// TODO: some sort of ParticleConfig struct
void particle_init(Particle* self, Vec2 position, Vec2 direction, f32 speed, f32 lifetime, SDL_Texture* texture);
bool particle_dead(Particle* self);

// TODO: ParticleEmitterConfig struct
void emitter_init(ParticleEmitter* self, u32 maxParticles, SDL_Texture* texture);
void emitter_free(ParticleEmitter* self);

void emitter_update(ParticleEmitter* self);
i32 emitter_get_next_available(ParticleEmitter* self);

void emitter_render(ParticleEmitter* self, Vec2* position);


#endif