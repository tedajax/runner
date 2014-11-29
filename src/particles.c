#include "particles.h"

void particle_init(Particle* self, Vec2 position, Vec2 direction, f32 speed, f32 lifetime, SDL_Texture* texture) {
    vec2_copy_to(&position, &self->position);
    vec2_copy_to(&direction, &self->direction);
    self->speed = speed;
    self->lifetime = lifetime;
    self->texture = texture;
    self->alpha = 255;
}

bool particle_dead(Particle* self) {
    return self->lifetime <= 0.f;
}

void emitter_init(ParticleEmitter* self, u32 maxParticles, SDL_Texture* texture) {
    self->capacity = maxParticles;
    self->particles = CALLOC(self->capacity, Particle);

    self->texture = texture;

    //self->simulateLocally = true;

    self->emitTimer = 0.f;
    self->emitInterval = 0.2f;
    self->particlesPerEmit = 10;
}

void emitter_free(ParticleEmitter* self) {
    free(self->particles);
    self->capacity = 0;
}

void emitter_update(ParticleEmitter* self) {
    self->emitTimer += globals.time.delta;

    for (u32 i = 0; i < self->capacity; ++i) {
        Particle* particle = &self->particles[i];

        if (particle_dead(particle)) {
            continue;
        }

        f32 scaledSpeed = particle->speed * globals.time.delta;
        Vec2 velocity;
        vec2_scale(&particle->direction, scaledSpeed, &velocity);
        vec2_add(&particle->position, &velocity, &particle->position);

        particle->lifetime -= globals.time.delta;
        particle->alpha = particle->lifetime / 1.f;
    }

    if (self->emitTimer < self->emitInterval) {
        return;
    }

    for (u32 i = 0; i < self->particlesPerEmit; ++i) {
        i32 index = emitter_get_next_available(self);
        ASSERT(index >= 0, "Reach maximum number of particles for emitter.  Change settings or make this work gracefully.");

        Particle* particle = &self->particles[index];

        particle_init(particle, vec2_zero(), vec2_rand_direction(), 100.f, 1.f, self->texture);
    }

    self->emitTimer = 0;
}

// TODO: this is going to be pretty slow so we'll want to do some sort of sorting or something to allow for some faster lookups
i32 emitter_get_next_available(ParticleEmitter* self) {
    for (u32 i = 0; i < self->capacity; ++i) {
        if (particle_dead(&self->particles[i])) {
            return i;
        }
    }

    return -1;
}

void emitter_render(ParticleEmitter* self, Vec2* position) {

    //TODO: not this
    int w;
    int h;
    SDL_QueryTexture(self->texture, NULL, NULL, &w, &h);

    for (u32 i = 0; i < self->capacity; ++i) {
        Particle* particle = &self->particles[i];
        
        if (particle_dead(particle)) {
            continue;
        }

        SDL_SetTextureAlphaMod(particle->texture, (u8)(particle->alpha * 255.f));

        SDL_Rect dest;
        dest.x = (int)particle->position.x + position->x;
        dest.y = (int)particle->position.y + position->y;
        dest.w = w;
        dest.h = h;

        SDL_RenderCopyEx(globals.renderer,
            particle->texture,
            NULL,
            &dest,
            0.f,
            NULL,
            SDL_FLIP_NONE);
    }
}