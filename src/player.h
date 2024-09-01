/**
 * @file player.h
 * @author Justin Tonkinson
 * @date 2024/08/15
 * @brief Player function prototypes.
 */

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "entity/entity.h"

#include <cglm/types-struct.h>

struct world_s;

/**
 * @brief Initializes the player entity.
 * @param pos starting position of player
 * @param dim dimensions of the player
 * @param world a pointer to the game world
 * @returns a pointer to the player entity
 */
entity_t *player_init(vec2s pos, vec2s dim, struct world_s *);

/**
 * @brief Releases the memory allocated for the player entity.
 * @param self a pointer to the player entity
 */
void player_destroy(entity_t *);

/**
 * @brief Updates the player entity.
 * @param self a pointer to the player entity
 * @param world a pointer to the game world
 * @param dt delta time
 */
void player_update(entity_t *, struct world_s *, float dt);

/* @brief Synchronizes the player entity with the tick rate.
 * @param self a pointer to the player entity
 * @param world a pointer to the game world
 * @param dt delta time
 */
void player_sync(entity_t *, struct world_s *, float dt);

/**
 * @brief Renders the player entity.
 * @param self a pointer to the player entity
 * @param world a pointer to the game world
 */
void player_render(entity_t *, struct world_s *);

#endif // ifndef _PLAYER_H_
