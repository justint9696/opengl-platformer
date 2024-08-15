#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "entity/entity.h"

#include <cglm/types-struct.h>

struct world_s;

/* Initializes the player entity. */
entity_t *player_init(vec2s pos, vec2s dim, struct world_s *);

/* Releases the memory allocated for the player entity. */
void player_destroy(entity_t *);

/* Updates the player entity. */
void player_update(entity_t *, struct world_s *, float dt);

/* Synchronizes the player entity with the tick rate. */
void player_sync(entity_t *, struct world_s *, float dt);

/* Renders the player entity. */
void player_render(entity_t *, struct world_s *);

#endif // ifndef _PLAYER_H_
