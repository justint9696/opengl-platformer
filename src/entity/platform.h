#ifndef _ENTITY_PLATFORM_H_
#define _ENTITY_PLATFORM_H_

#include "entity/entity.h"

#include <cglm/types-struct.h>

struct world_s;

/* Creates a platform and adds it to the world. */
entity_t *platform_create(vec2s, vec2s, struct world_s *);

#endif // ifndef _ENTITY_PLATFORM_H_
