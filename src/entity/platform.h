/**
 * @file platform.h
 * @author Justin Tonkinson
 * @date 2024/07/05
 * @brief Platform function prototypes.
 */

#ifndef _ENTITY_PLATFORM_H_
#define _ENTITY_PLATFORM_H_

#include "entity/entity.h"

#include <cglm/types-struct.h>

struct world_s;

/**
 * @brief Creates an platform and adds it to the world.
 * @param pos starting position of platform
 * @param dim dimensions of the platform
 * @param world a pointer to the game world
 * @returns a pointer to the platform entity
 */
entity_t *platform_create(vec2s, vec2s, struct world_s *);

#endif // ifndef _ENTITY_PLATFORM_H_
