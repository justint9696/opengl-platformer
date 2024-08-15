#ifndef _WORLD_WORLD_H_
#define _WORLD_WORLD_H_

#include "data/grid.h"
#include "entity/entity.h"
#include "game/camera.h"
#include "game/types.h"
#include "world/chunk.h"

#include <cglm/types-struct.h>
#include <glad/glad.h>
#include <stdbool.h>

typedef struct world_s {
    // uniform grid
    grid_t grid;

    // primary game camera
    camera_t camera;

    // reference to the player entity
    entity_t *player;

    // current chunk loaded in memory
    chunk_t chunk;

    // vertex buffer objects
    GLuint vao, vbo, ibo;
} world_t;

/* Initializes the game world. */
void world_init(world_t *);

/* Releases the memory allocated for the game world. */
void world_destroy(world_t *);

/* Updates all the entities in the game world. */
void world_update(world_t *, float dt);

/* Renders all the entities in the game world. */
void world_render(world_t *);

/* Synchonizes all the entities in the game world with the tick rate. */
void world_sync(world_t *, float dt);

/* Returns the screen coordinates of a position in world space. */
vec2s world_to_screen(const world_t *, vec2s pos);

/* Returns the world position of screen coordinates. */
vec2s screen_to_world(const world_t *, ivec2s pos);

/* Returns true if the world position is visible on screen. */
bool world_is_on_screen(const world_t *, vec2s pos);

/* Builds an array of potential colliders around an entity.
 * Returns the number of items added to the array. */
size_t world_get_colliders(world_t *, entity_t *, collider_t arr[], size_t len);

#endif // ifndef _WORLD_WORLD_H_
