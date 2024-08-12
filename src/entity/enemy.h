#ifndef _ENTITY_ENEMY_H_
#define _ENTITY_ENEMY_H_

#include "entity/entity.h"

#include <cglm/types-struct.h>

struct world_s;

/* Creates an enemy and adds it to the world. */
entity_t *enemy_create(vec2s pos, vec2s dim, struct world_s *);

#endif // ifndef _ENTITY_ENEMY_H_
