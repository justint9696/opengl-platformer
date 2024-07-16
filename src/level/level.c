#include "level/level.h"

#include "entity/table.h"
#include "util/log.h"
#include "util/util.h"

#include <assert.h>
#include <cglm/struct.h>

static void calculate_page_aabb(page_t *page, world_t *world, int index) {
    page->box.pos = glms_vec2_add(
        (vec2s) {
            .x = (index % world->chunk.dim.x) * CHUNK_SIZE,
            .y = (index % world->chunk.dim.y) * CHUNK_SIZE,
        },
        world->chunk.pos);
    log_debug("page %d at (%.2f, %.2f)\n", index, page->box.pos.x,
              page->box.pos.y);
    page->box.dim = glms_vec2_fill(CHUNK_SIZE);
}

static void load_pages(world_t *world, ldata_t *arr, size_t len, int index) {
    page_t *page = &world->chunk.pages[index];
    calculate_page_aabb(page, world, index);

    /* log_debug("loading %ld entities into chunk %d.\n", len, index); */
    for (size_t i = 0; i < len; i++) {
        ldata_t *data = &arr[i];
        create_fn_t f = table_lookup(data->type);

        entity_t *e = f(data->pos, data->dim, world);
        e->body.page = page;
    }
}

void level_import(level_t *self, world_t *world, const char *fpath) {
    FILE *fp = fopen(fpath, "rb");
    if (!fp) {
        fclose(fp);
        log_and_fail("Could not open file \"%s\"\n", fpath);
    }

    fseek(fp, 0, SEEK_SET);

    fread(&world->chunk.index, sizeof(int), 1, fp);
    fread(&world->chunk.dim, sizeof(ivec2s), 1, fp);

    glms_ivec2_print(world->chunk.dim, stdout);

    world->chunk.pos = (vec2s) {
        -CHUNK_SIZE,
        -CHUNK_SIZE,
    };

    ldata_t data;
    fread(&data, sizeof(ldata_t), 1, fp);

    create_fn_t f = table_lookup(data.type);
    world->player = f(data.pos, data.dim, world);

    struct {
        size_t n;
        ldata_t arr[CHUNK_MAX];
    } entities;

    int i = 0, idx = 0;
    int index = max(0, world->chunk.index - world->chunk.dim.x - 1);
    for (int n = 0; n < 3; n++) {
        for (; i < index; i++) {
            fread(&entities.n, sizeof(size_t), 1, fp);
            fread(&entities.arr, sizeof(ldata_t), entities.n, fp);
        }

        for (int j = 0; j < 3; j++, i++) {
            fread(&entities.n, sizeof(size_t), 1, fp);
            fread(&entities.arr, sizeof(ldata_t), entities.n, fp);
            load_pages(world, entities.arr, entities.n, idx++);
        }

        index = world->chunk.dim.x + index - 1;
    }

    fclose(fp);
}

void level_export(const level_t *self, const world_t *world,
                  const char *fpath) {
    /* FILE *fp = fopen(fpath, "wb+"); */
    /* if (!fp) { */
    /*     log_and_fail("Could not open file %s\n", fpath); */
    /* } */

    /* fwrite(&world->chunk.origin, sizeof(vec2s), 1, fp); */

    /* size_t len; */
    /* len = array_len(world->entities); */
    /* fwrite(&len, sizeof(size_t), 1, fp); */

    /* for (size_t i = 0; i < len; i++) { */
    /*     entity_t *entity = &world->entities[i]; */
    /*     assert(entity); */

    /*     ldata_t data = { */
    /*         .type = entity->type, */
    /*         .box = entity->body.box, */
    /*     }; */

    /*     fwrite(&data, sizeof(ldata_t), 1, fp); */
    /* } */

    /* fclose(fp); */
    /* log_debug("Successfully exported level data to file \"%s\".\n", fpath);
     */
}

void level_swap_chunks(level_t *self, world_t *world, uint32_t idx);
