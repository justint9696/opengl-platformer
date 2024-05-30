#ifndef _ENTITY_ENEMY_H_
#define _ENTITY_ENEMY_H_

#include "entity/entity.h"

#include <cglm/types-struct.h>

struct world_s;

entity_t *enemy_create(vec2s pos, vec2s dim, struct world_s *);

#endif
