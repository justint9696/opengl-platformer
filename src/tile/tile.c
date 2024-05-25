#include "tile/tile.h"

#include "data/array.h"
#include "world/world.h"

tile_t *tile_create(void *data, world_t *world) {
    uint32_t id = array_push((void **)&world->tiles, data);
    tile_t *self = array_get(world->tiles, id);

    self->id = id;
    if (self->init != NULL) {
        self->init(self, world);
    }

    return self;
}

void tile_destroy(tile_t *self, world_t *world) {
    if (self->destroy != NULL) {
        self->destroy(self, world);
    }
    array_remove(world->tiles, self);
}

void tile_render(tile_t *self, world_t *world) {
    if (self->render != NULL) {
        self->render(self, world);
    }
}
