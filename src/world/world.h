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

void world_init(world_t *);
void world_destroy(world_t *);
void world_update(world_t *, float dt);
void world_render(world_t *);
void world_sync(world_t *, float dt);

vec2s world_to_screen(const world_t *, vec2s pos);
vec2s screen_to_world(const world_t *, ivec2s pos);
bool world_is_on_screen(const world_t *, vec2s pos);

size_t world_get_colliders(world_t *, entity_t *, collider_t *arr, size_t len);

#endif // ifndef _WORLD_WORLD_H_
