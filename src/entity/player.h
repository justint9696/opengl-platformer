#ifndef _ENTITY_PLAYER_H_
#define _ENTITY_PLAYER_H_

#include "entity/entity.h"

#include <cglm/types-struct.h>

struct world_s;

entity_t *player_create(vec2s pos, vec2s dim, struct world_s *);

#endif
