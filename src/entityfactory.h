#ifndef RUNNER_ENTITY_FACTORY_H
#define RUNNER_ENTITY_FACTORY_H

#include "entity.h"
#include "entitymanager.h"
#include "components.h"
#include "systems.h"

#include <SDL2/SDL.h>

Entity* entity_create_player(EntityManager* entityManager, Vec2 position, SDL_Texture* texture);

#endif