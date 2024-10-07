/**
 * @file aabb.h
 * @author Justin Tonkinson
 * @date 2024/05/24
 * @brief Axis-aligned bounding box (AABB) data structure and function
 * prototypes.
 */

#ifndef _UTIL_AABB_H_
#define _UTIL_AABB_H_

#include <cglm/types-struct.h>
#include <stdbool.h>

typedef struct {
    vec2s pos, dim;
} aabb_t, box_t;

/**
 * @brief Returns the center point of an aabb.
 * @param self a pointer to an aabb
 * @returns the center of the aabb
 */
vec2s box_center(const box_t *);

/**
 * @brief Creates a broad-phase bounding box (bbb) along the provided axis.
 * @param self a pointer to an aabb
 * @param vel velocity of the object
 * @param axis movement axis
 * @param dt delta time
 */
void bbb_create(box_t *, vec2s vel, int axis, float dt);

/**
 * @brief Determines if two aabb's are colliding or overlapping.
 * @param a a pointer to the first aabb
 * @param b a pointer to the second aabb
 * @returns true if the two aabb's overlap or false otherwise
 */
bool aabb_collision_2d(const box_t *, const box_t *);

/**
 * @brief Determines if a position intersects an aabb.
 * @param self a pointer to an aabb
 * @param pos position in world space
 * @returns true if the position intersects the aabb or false otherwise
 */
bool aabb_intersect_2d(const box_t *, vec2s pos);

#endif // ifndef _UTIL_AABB_H_
