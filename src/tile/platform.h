#ifndef _TILE_PLATFORM_H_
#define _TILE_PLATFORM_H_

#include "tile/tile.h"

#include <cglm/types-struct.h>

struct world_s;

tile_t *platform_create(vec2s pos, vec2s dim, struct world_s *);

#endif // _TILE_PLATFORM_H_
