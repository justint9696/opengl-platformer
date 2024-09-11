/**
 * @brief chunk.h
 * @author Justin Tonkinson
 * @date 2024/07/04
 * @brief Chunk structures and function prototypes.
 */

#ifndef _WORLD_CHUNK_H_
#define _WORLD_CHUNK_H_

#include "entity/entity.h"

#include <stdint.h>

#define CHUNK_SIZE  512
#define CHUNK_MAX   128
#define CHUNK_WIDTH 3

/** @brief Represents the data that is stored in a region of the current chunk.
 */
typedef struct page_s {
    /** @brief Index used to get surrounding pages. */
    int32_t index;

    /** @brief The bounds of the current page. */
    union {
        box_t box;
        struct {
            vec2s pos, dim;
        };
    };

    /** @brief Array of entities within the current page. */
    entity_t *entities;
} page_t;

/** @brief Represents a chunk of memory within a region of the game world. */
typedef struct chunk_s {
    /** @brief The index of the center chunk. */
    int32_t index;

    /** @brief The absolute bounds of the level. */
    box_t box;

    /** @brief The current position in world space. */
    vec2s pos;

    /** @brief The current pages loaded into memory. */
    page_t pages[9];
} chunk_t;

/**
 * @brief Initializes the world chunk.
 * @param self a pointer to a chunk
 */
void chunk_init(chunk_t *);

/**
 * @brief Releases the memory allocated for the world chunk.
 * @param self a pointer to a chunk
 */
void chunk_destroy(chunk_t *);

/**
 * @brief Renders the bounds of the world chunk.
 * @param self a pointer to a chunk
 */
void chunk_render(chunk_t *);

/**
 * @brief Returns the page asscoiated with the given position.
 * @param self a pointer to a chunk
 * @param pos a position in world space
 * @returns a pointer to the page that contains the given position
 */
page_t *chunk_page_from_pos(chunk_t *, vec2s pos);

/**
 * @brief Returns the chunk index associated with the given position.
 * @param self a pointer to a chunk
 * @param pos a position in world space
 * @returns the index of the page within the entire world that contains the
 * given position or -1 if the position is invalid
 */
int chunk_index_from_pos(chunk_t *, vec2s pos);

/**
 * @brief Returns a free block of memory that is initialized as an dynamic
 * array.
 * @param self a pointer to a chunk
 * @param page a pointer to a page
 * @returns a pointer to an array
 */
void *chunk_request_page(chunk_t *, page_t *);

/**
 * @brief Releases an occupied block of memory and marks it as free.
 * @param self a pointer to a chunk
 * @param page a pointer to a page
 */
void chunk_release_page(chunk_t *, page_t *);

#endif // ifndef _WORLD_CHUNK_H_
