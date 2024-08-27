/**
 * @file enemy.h
 * @author Justin Tonkinson
 * @date 2024/05/24
 * @brief Enemy function prototypes.
 */

#ifndef _ENTITY_ENEMY_H_
#define _ENTITY_ENEMY_H_

#include "entity/entity.h"

#include <cglm/types-struct.h>

struct world_s;

/**
 * @brief Creates an enemy and adds it to the world. 
 * @param pos starting position of enemy
 * @param dim dimensions of the enemy
 * @returns a pointer to the enemy entity
 */
entity_t *enemy_create(vec2s pos, vec2s dim, struct world_s *);

#endif // ifndef _ENTITY_ENEMY_H_
