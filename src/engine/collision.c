#include "data/array.h"
#include "entity/entity.h"
#include "world/world.h"

#include <assert.h>
#include <cglm/struct.h>
#include <stdbool.h>

static void resolve_collision(entity_t *self, world_t *world, box_t *box,
                              vec2s *movement, short axis) {
    // store current position
    vec2s prev_pos = world_to_screen(world, self->body.pos);

    // set position to entry point of collision along the current axis
    if (movement->raw[axis] < 0.000001f) {
        self->body.pos.raw[axis] = box->pos.raw[axis] + box->dim.raw[axis];
    } else if (movement->raw[axis] > 0.000001f) {
        self->body.pos.raw[axis]
            = box->pos.raw[axis] - self->body.dim.raw[axis];
    }

    // update position within uniform grid
    vec2s pos = world_to_screen(world, self->body.pos);
    if (self->body.solid) {
        grid_update(&world->grid, &self, prev_pos, pos);
    }

    // stop further movement along collision axis
    movement->raw[axis] = 0.f;
}

static vec2s try_move(entity_t *self, world_t *world, void *arr[], size_t len,
                      float dt) {
#ifdef _DEBUG
    // clear debug lines array
    array_clear(self->debug.lines);
#endif

    vec2s movement = glms_vec2_make(self->body.vel.raw);

    // check for collision with entities
    for (size_t i = 0; i < len; i++) {
        entity_t *tmp = arr[i];
        for (short i = 0; i < 2; i++) {
            if (fabsf(movement.raw[i]) <= 0.000001f)
                continue;

            box_t bbb = self->body.box;
            bbb_create(&bbb, movement, i, dt);
            if (aabb_collision_2d(&bbb, &tmp->body.box)) {
                resolve_collision(self, world, &tmp->body.box, &movement, i);
                if (tmp->body.collision_callback) {
                    tmp->body.collision_callback(tmp, self);
                }
            }
        }
    }

    // check for collision with tiles
    for (short i = 0; i < 2; i++) {
        if (fabsf(movement.raw[i]) <= 0.000001f)
            continue;

        box_t bbb = self->body.box;
        bbb_create(&bbb, movement, i, dt);

        vec2s pos = bbb.pos;
        pos.raw[i] += (movement.raw[i] > 0.f) ? bbb.dim.raw[i] : 0.f;

        tile_t *tmp = kdtree_nearest(&world->kdtree, pos.raw);
        assert(tmp);

#ifdef _DEBUG
        vec2s center = box_center(&self->body.box);
        array_push((void **)&self->debug.lines, &center);
        array_push((void **)&self->debug.lines, &tmp->body.pos);
#endif

        if (aabb_collision_2d(&bbb, &tmp->body.box)) {
            resolve_collision(self, world, &tmp->body.box, &movement, i);
            if (tmp->body.collision_callback) {
                tmp->body.collision_callback(tmp, self);
            }
        }
    }

    return movement;
}

void collision_sync(entity_t *self, world_t *world, float dt) {
    // get possible colliders from uniform grid
    entity_t *arr[64];
    size_t n = world_get_colliders(world, self, arr, 64);

    // attempt to move entity
    vec2s movement = try_move(self, world, (void **)arr, n, dt);

    // entity is grounded if downward velocity was stopped
    self->body.grounded = self->body.vel.y < 0 && movement.y == 0;

    self->body.vel = glms_vec2_make(movement.raw);
}
