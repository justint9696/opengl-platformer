#ifndef _ENTITY_PLAYER_H_
#define _ENTITY_PLAYER_H_

#include "entity/entity.h"

#include <cglm/types-struct.h>

struct world_s;

/* Creates a player and adds it to the world. */
entity_t *player_create(vec2s pos, vec2s dim, struct world_s *);

#endif // ifndef _ENTITY_PLAYER_H_
