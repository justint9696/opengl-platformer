#include "tile/tile.h"

#include "data/array.h"
#include "util/log.h"
#include "world/world.h"

#include <assert.h>
#include <stdlib.h>

static int sort_x(const void *a, const void *b) {
    const tile_t *x = *(const tile_t **)a;
    const tile_t *y = *(const tile_t **)b;

    assert(x && y);

    if (x->body.pos.x < y->body.pos.x)
        return -1;
    else if (x->body.pos.x > y->body.pos.x)
        return 1;
    return 0;
}

static int sort_y(const void *a, const void *b) {
    const tile_t *x = *(const tile_t **)a;
    const tile_t *y = *(const tile_t **)b;

    assert(x && y);

    if (x->body.pos.y < y->body.pos.y)
        return -1;
    else if (x->body.pos.y > y->body.pos.y)
        return 1;
    return 0;
}

void tile_sort(void *arr, size_t len, short axis) {
    switch (axis) {
        case 0:
            qsort(arr, len, sizeof(tile_t *), sort_x);
            break;
        case 1:
            qsort(arr, len, sizeof(tile_t *), sort_y);
            break;
        default:
            log_and_fail("Invalid sorting axis %d.\n", axis);
            break;
    }
}

tile_t *tile_create(void *data, world_t *world) {
    uint32_t id = array_push(world->tiles, data);
    tile_t *self = array_get(world->tiles, id);

    self->id = id;
    if (self->init) {
        self->init(self, world);
    }

    return self;
}

void tile_destroy(tile_t *self, world_t *world) {
    if (self->destroy) {
        self->destroy(self, world);
    }

    array_remove(world->tiles, self);
}

void tile_render(tile_t *self, world_t *world) {
    if (self->render) {
        self->render(self, world);
    }
}
