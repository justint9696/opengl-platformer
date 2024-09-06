/**
 * @brief world.h
 * @author Justin Tonkinson
 * @date 2024/07/04
 * @brief World structure and function prototypes.
 */

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

/** @brief World data structure. */
typedef struct world_s {
    /** @brief Uniform grid. */
    grid_t grid;

    /** @brief The primary game camera. */
    camera_t camera;

    /** @brief A pointer to the player entity. */
    entity_t(*player);

    /** @brief The chunk of the current pages loaded in memory. */
    chunk_t chunk;

    /** @brief Vertex buffer objects. */
    GLuint vao, vbo, ibo;
} world_t;

/**
 * @brief Initializes the game world.
 * @param self a pointer to the game world
 */
void world_init(world_t *);

/**
 * @brief Releases the memory allocated for the game world.
 * @param self a pointer to the game world
 */
void world_destroy(world_t *);

/**
 * @brief Updates all the entities in the game world.
 * @param self a pointer to the game world
 * @param dt delta time
 */
void world_update(world_t *, float dt);

/**
 * @brief Renders all the entities in the game world.
 * @param self a pointer to the game world
 */
void world_render(world_t *);

/**
 * @brief Synchonizes all the entities in the game world with the tick rate.
 * @param self a pointer to the game world
 * @param dt delta time
 */
void world_sync(world_t *, float dt);

/**
 * @brief Returns the screen coordinates of a position in world space.
 * @param self a pointer to the game world
 * @param pos a position in world space
 * @returns the screen position of the given world position
 */
vec2s world_to_screen(const world_t *, vec2s pos);

/**
 * @brief Returns the world position of screen coordinates.
 * @param self a pointer to the game world
 * @param pos a position in screen space
 * @returns the world position of the given screen position
 */
vec2s screen_to_world(const world_t *, ivec2s pos);

/**
 * @brief Returns true if the world position is visible on screen.
 * @param self a pointer to the game world
 * @returns true if the position is visible on screen, false otherwise
 */
bool world_is_on_screen(const world_t *, vec2s pos);

/**
 * @brief Builds an array of potential colliders around an entity.
 * @param self a pointer to the game world
 * @param entity a pointer to the current entity
 * @param arr the destination array
 * @param len the capacity of the array
 * @returns the number of items added to the array.
 */
size_t world_get_colliders(world_t *, entity_t *, collider_t arr[], size_t len);

#endif // ifndef _WORLD_WORLD_H_
