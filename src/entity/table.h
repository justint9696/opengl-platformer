/**
 * @file table.h
 * @author Justin Tonkinson
 * @date 2024/07/05
 * @brief Entity lookup table function prototypes.
 */

#ifndef _ENTITY_TABLE_H_
#define _ENTITY_TABLE_H_

#include "world/world.h"

#include <stdint.h>

/** @brief Entity create function. */
typedef void *(*create_fn_t)(vec2s, vec2s, world_t *);

/**
 * @brief Returns the create function for an entity type.
 * @param type the type of entity
 * @returns a pointer to the entity's create function
 */
create_fn_t table_lookup(entity_type_t);

#endif // ifndef _ENTITY_TABLE_H_
