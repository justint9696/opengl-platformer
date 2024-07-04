#include "level/level.h"

#include "data/array.h"
#include "entity/entity.h"
#include "tile/tile.h"
#include "util/log.h"
#include "world/chunk.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

void level_import(level_t *self, world_t *world, const char *fname) {
    FILE *fp = fopen(fname, "rb");
    if (!fp) {
        log_and_fail("Could not open file %s\n", fname);
    }

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    assert(size);

    fseek(fp, 0, SEEK_SET);

    fread(&world->chunk.origin, sizeof(vec2s), 1, fp);

    struct { 
        size_t n;
        ldata_t arr[CHUNK_MAX];
    } entities, tiles;

    fread(&entities.n, sizeof(size_t), 1, fp);
    fread(&entities.arr, sizeof(ldata_t), entities.n, fp);

    fread(&tiles.n, sizeof(size_t), 1, fp);
    fread(&tiles.arr, sizeof(ldata_t), tiles.n, fp);

    for (size_t i = 0; i < entities.n; i++) {
        ldata_t *data = &entities.arr[i];
        log_debug("arr[%ld] = { %d, (%.2f, %.2f), (%.2f, %.2f)\n",
                  i, data->type, 
                  data->pos.x, data->pos.y,
                  data->dim.x, data->dim.y);
    }

    for (size_t i = 0; i < tiles.n; i++) {
        ldata_t *data = &tiles.arr[i];
        log_debug("arr[%ld] = { %d, (%.2f, %.2f), (%.2f, %.2f)\n",
                  i, data->type, 
                  data->pos.x, data->pos.y,
                  data->dim.x, data->dim.y);
    }

    fclose(fp);
}

void level_export(const level_t *self, const world_t *world,
                  const char *fname) {
    FILE *fp = fopen(fname, "wb+");
    if (!fp) {
        log_and_fail("Could not open file %s\n", fname);
    }

    fwrite(&world->chunk.origin, sizeof(vec2s), 1, fp);

    size_t len;
    len = array_len(world->entities);
    fwrite(&len, sizeof(size_t), 1, fp);

    for (size_t i = 0; i < len; i++) {
        entity_t *entity = &world->entities[i];
        assert(entity);

        ldata_t data = {
            .type = entity->type,
            .box = entity->body.box,
        };

        fwrite(&data, sizeof(ldata_t), 1, fp);
    }

    len = array_len(world->tiles);
    fwrite(&len, sizeof(size_t), 1, fp);

    for (size_t i = 0; i < len; i++) {
        tile_t *tile = &world->tiles[i];
        assert(tile);

        ldata_t data = {
            .type = tile->type,
            .box = tile->body.box,
        };

        fwrite(&data, sizeof(ldata_t), 1, fp);
    }

    fclose(fp);
    log_debug("Successfully exported level data to file \"%s\".\n", fname);
}

void level_swap_chunks(level_t *self, world_t *world, uint32_t idx);
