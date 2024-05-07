#pragma once

#include "entity/entity.h"

#include <cglm/types-struct.h>

struct world_s;

entity_s *player_create(vec2s pos, vec2s dim, struct world_s *);
