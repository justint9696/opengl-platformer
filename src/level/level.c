#include "level/level.h"

#include "data/array.h"
#include "entity/table.h"
#include "util/log.h"
#include "util/util.h"

#include <assert.h>
#include <cglm/struct.h>
#include <errno.h>
#include <string.h>

/*
 * level.dat file format
 * int              origin;
 * ivec2s           dchunk; // (width, height)
 * ldata_t          player;
 *
 * for (i = 0; i < nchunk; i++) {
 *     size_t       nent;
 *     ldata_t      entities[nent];
 * }
 */

// corresponding to entities and tiles
typedef struct {
    int type;
    union {
        box_t box;
        struct {
            vec2s pos, dim;
        };
    };
} ldata_t;

static struct {
    // array of position offsets of pages within level data
    int *offsets;

    // level file pointer
    FILE *fp;
} level;

static void calculate_page_aabb(page_t *page, world_t *world, int index) {
    page->box.pos = glms_vec2_add(
        (vec2s) {
            .x = (index % CHUNK_WIDTH) * CHUNK_SIZE,
            .y = floorf(1.f * index / CHUNK_WIDTH) * CHUNK_SIZE,
        },
        world->chunk.pos);
    page->box.dim = glms_vec2_fill(CHUNK_SIZE);
}

static void load_pages(world_t *world, ldata_t *arr, size_t len, int index) {
    page_t *page = &world->chunk.pages[index];
    calculate_page_aabb(page, world, index);

    /* log_debug("loading %ld entities into page %d\n", len, index); */

    for (size_t i = 0; i < len; i++) {
        ldata_t *data = &arr[i];
        create_fn_t f = table_lookup(data->type);
        f(data->pos, data->dim, world);
    }
}

void level_import(world_t *world, const char *fpath) {
    level.fp = fopen(fpath, "rb");
    if (!level.fp) {
        log_and_fail("Could not open file %s: %s\n", fpath, strerror(errno));
    }

    fseek(level.fp, 0, SEEK_SET);
    fread(&world->chunk.index, sizeof(int), 1, level.fp);
    fread(&world->chunk.dim, sizeof(ivec2s), 1, level.fp);

    // FIXME: calculate the actual position based on the chunk index
    world->chunk.pos = glms_vec2_fill(-CHUNK_SIZE);

    ldata_t data;
    fread(&data, sizeof(ldata_t), 1, level.fp);

    create_fn_t f = table_lookup(data.type);
    world->player = f(data.pos, data.dim, world);

    struct {
        size_t n;
        ldata_t arr[CHUNK_MAX];
    } entities;

    int32_t size = (world->chunk.dim.x * world->chunk.dim.y);
    level.offsets = array_alloc(sizeof(int), size);

    int index = 0;
    int32_t target = max(0, world->chunk.index - world->chunk.dim.x - 1);
    for (int32_t i = 0; i < size; i++) {
        fread(&entities.n, sizeof(size_t), 1, level.fp);
        fread(&entities.arr, sizeof(ldata_t), entities.n, level.fp);

        size_t offset = ftell(level.fp);
        array_push(level.offsets, &offset);

        // have all chunks been loaded or should the page be loaded
        if ((index >= 9) || ((target - i) > 0))
            continue;

        // have all 3 pages been loaded
        if ((i - target) == 2)
            target = world->chunk.dim.x + target;

        load_pages(world, entities.arr, entities.n, index++);
    }
}

void level_export(const world_t *world, const char *fpath) {
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

void level_shutdown() {
    array_free(level.offsets);

    assert(level.fp);
    fclose(level.fp);
}

static void shift_pages(world_t *world, int indices[], int it) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            int32_t index = (indices[i] + (it * j));
            page_t *page = &world->chunk.pages[index];

            log_debug("copy page %d into %d\n", index,
                      indices[i] + (it * (j + 1)));

            index = ((indices[i] + (it * (j + 1))));
            array_copy(page->entities, world->chunk.pages[index].entities);
            
            calculate_page_aabb(page, world, page->index);
        }
    }
}

static void replace_pages(world_t *world, int indices[], int it) {
    struct {
        size_t n;
        ldata_t arr[CHUNK_MAX];
    } entities;

    for (int i = 0; i < 3; i++) {
        page_t *page = &world->chunk.pages[indices[i]];
        array_clear(page->entities);

        vec2s pos
            = glms_vec2_negate(glms_vec2_div(page->box.pos, world->chunk.pos));
        int32_t index
            = (world->chunk.index + (world->chunk.dim.x * pos.y) + pos.x) + it;

        log_debug("requesting index %d for page %d\n", index, indices[i]);

        int32_t offset = *(int32_t *)array_get(level.offsets, index);
        fseek(level.fp, offset, SEEK_SET);

        fread(&entities.n, sizeof(size_t), 1, level.fp);
        fread(&entities.arr, sizeof(ldata_t), entities.n, level.fp);

        load_pages(world, entities.arr, entities.n, indices[i]);
    }

    world->chunk.index += it;
    log_debug("moved to chunk: %d\n", world->chunk.index);
}

void level_swap_chunks(world_t *world, int index) {
    assert(level.fp);

    enum {
        TOP_LEFT = -4,
        TOP_MIDDLE = -3,
        TOP_RIGHT = -2,
        LEFT = -1,
        CENTER = 0,
        RIGHT = 1,
        BOTTOM_LEFT = 2,
        BOTTOM_MIDDLE = 3,
        BOTTOM_RIGHT = 4,
    };

    // move the pages into correct indices within chunk->pages[]
    // load the new pages from the level data file based on indicies and offsets
    switch (index) {
        case TOP_LEFT:
        case TOP_MIDDLE:
        case TOP_RIGHT:
            /* shift_pages_down(world, index); */
            break;
    }

    switch (index) {
        case BOTTOM_LEFT:
        case BOTTOM_MIDDLE:
        case BOTTOM_RIGHT:
            /* shift_pages_up(world, index); */
            break;
    }

    switch (index) {
        case TOP_LEFT:
        case BOTTOM_LEFT:
        case LEFT:
            log_debug("shifting pages right\n");
            shift_pages(world, (int[]) { 2, 5, 8 }, -1);
            replace_pages(world, (int[]) { 0, 3, 6 }, -1);
            world->chunk.pos.x -= CHUNK_SIZE;
            break;
    }

    switch (index) {
        case TOP_RIGHT:
        case BOTTOM_RIGHT:
        case RIGHT:
            log_debug("shifting pages left\n");
            shift_pages(world, (int[]) { 0, 3, 6 }, 1);
            replace_pages(world, (int[]) { 2, 5, 8 }, 1);
            world->chunk.pos.x += CHUNK_SIZE;
            break;
    }
}
