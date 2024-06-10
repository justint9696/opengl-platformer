#ifndef _WORLD_WORLD_H_
#define _WORLD_WORLD_H_

#include "data/grid.h"
#include "data/kdtree.h"
#include "entity/entity.h"
#include "game/camera.h"
#include "tile/tile.h"

#include <cglm/types-struct.h>
#include <glad/glad.h>

typedef struct world_s {
    // uniform grid
    grid_t grid;

    // primary game camera
    camera_t camera;

    // reference to player entity
    entity_t *p_player;

    // kdtree of static world geometry
    kdtree_t kdtree;

    // array of all entities
    entity_t *entities;

    // array of all tiles
    tile_t *tiles;

    GLuint vao, vbo, ibo;
} world_t;

void world_init(world_t *);
void world_destroy(world_t *);
void world_update(world_t *, float dt);
void world_render(world_t *);
void world_sync(world_t *, float dt);

vec2s world_to_screen(const world_t *, vec2s pos);
size_t world_get_colliders(world_t *, entity_t *, entity_t *arr[], size_t len);

#endif
