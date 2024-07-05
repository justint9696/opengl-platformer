#include "level/level.h"

#include "data/array.h"
#include "entity/entity.h"
#include "entity/table.h"
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
    } entities;

    fread(&entities.n, sizeof(size_t), 1, fp);
    fread(&entities.arr, sizeof(ldata_t), entities.n, fp);

    // player should always be at index 0
    ldata_t *data = &entities.arr[0];
    create_fn_t create = table_lookup(data->type);
    world->player = create(data->pos, data->dim, world);

    for (size_t i = 1; i < entities.n; i++) {
        data = &entities.arr[i];
        create = table_lookup(data->type);
        create(data->pos, data->dim, world);
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

    fclose(fp);
    log_debug("Successfully exported level data to file \"%s\".\n", fname);
}

void level_swap_chunks(level_t *self, world_t *world, uint32_t idx);
