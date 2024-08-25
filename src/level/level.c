#include "level/level.h"

#include "data/array.h"
#include "entity/table.h"
#include "player.h"
#include "util/assert.h"
#include "world/chunk.h"

#include <assert.h>
#include <cglm/struct.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

static level_t level;

/* Creates the page bounds based on the chunk position and page index. */
static void page_calculate_aabb(page_t *page, world_t *world) {
    page->pos = glms_vec2_add(
        (vec2s) {
            .x = (page->index % CHUNK_WIDTH) * CHUNK_SIZE,
            .y = (2 - floorf((float)page->index / CHUNK_WIDTH)) * CHUNK_SIZE,
        },
        world->chunk.pos);
    page->dim = glms_vec2_fill(CHUNK_SIZE);
}

/* Loads level data into the provided page. */
static void page_load_data(page_t *page, world_t *world, ldata_t arr[],
                           size_t len) {
    size_t n = 0;

    for (size_t i = 0; i < len; i++) {
        ldata_t *data = &arr[i];

        // FIXME: if the player leaves the bounds of the world, a segmentation
        // fault occurs
        page_t *tmp = chunk_page_from_pos(&world->chunk, data->pos);
        if (tmp->index != page->index) {
            n++;
        }

        create_fn_t fn = table_lookup(data->type);
        assert(fn);

        fn(data->pos, data->dim, world);
    }
    if (n) {
        log_fatal("%ld entities were loaded into the wrong page\n", n);
    }
}

void level_shutdown(world_t *world) {
    if (level.fp)
        return;

    fclose(level.fp);
    array_free(level.offsets);

    assert(world->player);
    player_destroy(world->player);
}

void level_import(world_t *world, const char *fpath) {
    FILE *fp = fopen(fpath, "rb");
    XASSERT(fp, "Could not open file `%s`: %s.\n", fpath, strerror(errno));

    level.fp = tmpfile();
    XASSERT(level.fp, "Failed to create temp file: `%s`\n", strerror(errno));

    fseek(fp, 0, SEEK_SET);
    fread(&world->chunk.index, sizeof(int), 1, fp);
    fread(&world->chunk.dim, sizeof(ivec2s), 1, fp);

    // FIXME: calculate the actual position based on the chunk index
    world->chunk.pos = glms_vec2_fill(-CHUNK_SIZE);

    ldata_t data;
    fread(&data, sizeof(ldata_t), 1, fp);
    fwrite(&data, sizeof(ldata_t), 1, level.fp);

    world->player = player_init(data.pos, data.dim, world);

    struct {
        size_t n;
        ldata_t arr[CHUNK_MAX];
    } entities;

    int size = (world->chunk.dim.x * world->chunk.dim.y);
    level.offsets = array_alloc(sizeof(uint64_t), size);

    // current page index
    int index = 0;

    // top left page of starting chunk
    int target = glm_max(0, world->chunk.index - world->chunk.dim.x - 1);

    for (int i = 0; i < size; i++) {
        // offset within level file
        uint64_t offset = ftell(level.fp);
        array_push(level.offsets, &offset);

        // read data from level file
        fread(&entities.n, sizeof(size_t), 1, fp);
        fread(&entities.arr, sizeof(ldata_t), entities.n, fp);

        // write data to temporary file with padding
        fwrite(&entities.n, sizeof(size_t), 1, level.fp);
        fwrite(&entities.arr, sizeof(ldata_t), CHUNK_MAX, level.fp);

        // have all the pages been loaded or is the page not part of the chunk
        if ((index >= 9) || ((target - i) > 0))
            continue;

        // have all 3 pages been loaded
        if ((i - target) >= 2)
            target = world->chunk.dim.x + target;

        /* log_debug("Loading index %d into memory\n", i); */

        // load pages into memory
        page_t *page = &world->chunk.pages[index++];
        page_calculate_aabb(page, world);
        page_load_data(page, world, entities.arr, entities.n);
    }

    fclose(fp);
}

void level_export(world_t *world, const char *fpath) {
    assert(level.fp);

    FILE *fp = fopen(fpath, "wb+");
    XASSERT(fp, "Could not open file `%s`: %s\n.", fpath, strerror(errno));

    fseek(fp, 0, SEEK_SET);
    fwrite(&world->chunk.index, sizeof(int), 1, fp);
    fwrite(&world->chunk.dim, sizeof(ivec2s), 1, fp);

    fwrite(
        &(ldata_t) {
            .type = world->player->type,
            .box = world->player->body.box,
        },
        sizeof(ldata_t), 1, fp);

    struct {
        size_t n;
        ldata_t arr[CHUNK_MAX];
    } entities;

    size_t len = array_len(level.offsets);
    for (size_t i = 0; i < len; i++) {
        // set file pointer to offset within file
        uint64_t offset = level.offsets[i];
        fseek(level.fp, offset, SEEK_SET);

        // read data at temp file offset
        fread(&entities.n, sizeof(size_t), 1, level.fp);
        fread(&entities.arr, sizeof(ldata_t), entities.n, level.fp);

        // write data to export file without padding
        fwrite(&entities.n, sizeof(size_t), 1, fp);
        fwrite(&entities.arr, sizeof(ldata_t), entities.n, fp);
    }

    fclose(fp);
}

/* Shifts the provided page indicies around the iterator. */
static void pages_shift(world_t *world, uint32_t indices[], int it) {
    // iterate through all the indices
    for (int i = 0; i < 3; i++) {
        uint32_t index = indices[i];
        page_t *page = &world->chunk.pages[index];

        // remove entities from the world
        ldata_t data[CHUNK_MAX];
        size_t len = array_len(page->entities);

        /* log_debug("Releasing %ld entities from page %d\n", len, page->index);
         */
        for (size_t n = 0; n < len; n++) {
            entity_t *entity = &page->entities[0];
            data[n] = (ldata_t) {
                .type = entity->type,
                .box = entity->body.box,
            };

            entity_destroy(entity, world);
        }

        int idx = chunk_index_from_pos(&world->chunk, page->pos);
        uint64_t offset = level.offsets[idx];
        fseek(level.fp, offset, SEEK_SET);

        fwrite(&len, sizeof(size_t), 1, level.fp);
        fwrite(&data, sizeof(ldata_t), CHUNK_MAX, level.fp);

        // mark page block as free
        chunk_release_page(&world->chunk, page);

        // shift the pages around the iterator
        for (int j = 1; j < 3; j++) {
            indices[i] += it;
            index = indices[i];

            page_t *tmp = &world->chunk.pages[index];

            page->entities = tmp->entities;
            page->box = tmp->box;

            page = tmp;
        }
    }
}

/* Replaces the provided page indicies around the iterator. */
static void pages_replace(world_t *world, uint32_t indices[], int it) {
    struct {
        size_t n;
        ldata_t arr[CHUNK_MAX];
    } entities;

    // iterate through all the indices
    for (int i = 0; i < 3; i++) {
        page_t *page = &world->chunk.pages[indices[i]];

        // request block of memory
        page->entities = chunk_request_page(&world->chunk, page);
        page_calculate_aabb(page, world);

        int index = chunk_index_from_pos(&world->chunk, page->pos);
        if (index == -1)
            continue;

        /* log_debug("Requesting index %d for page %d\n", index, page->index);
         */
        uint64_t offset = level.offsets[index];
        fseek(level.fp, offset, SEEK_SET);

        fread(&entities.n, sizeof(size_t), 1, level.fp);
        fread(&entities.arr, sizeof(ldata_t), entities.n, level.fp);

        /* log_debug("Loading %ld entities into page %d\n", entities.n, */
        /*           page->index); */

        page_load_data(page, world, entities.arr, entities.n);
    }
}

/* Swaps the provided page indicies horizontally around the iterator. */
static void pages_swap_horz(world_t *world, uint32_t indices[], int it) {
    pages_shift(world, indices, it);

    world->chunk.index += it;
    world->chunk.pos.x += (it * CHUNK_SIZE);

    pages_replace(world, indices, it);
}

void level_swap_pages(world_t *world, page_t *page) {
    assert(level.fp);

    enum {
        TOP_LEFT,
        TOP_MIDDLE,
        TOP_RIGHT,
        LEFT,
        CENTER,
        RIGHT,
        BOTTOM_LEFT,
        BOTTOM_MIDDLE,
        BOTTOM_RIGHT,
    };

    // calculate the direction the pages are moving
    uint32_t index = page->index;

    switch (index) {
        case TOP_LEFT:
        case TOP_MIDDLE:
        case TOP_RIGHT:
            // shift pages down
            break;
    }

    switch (index) {
        case BOTTOM_LEFT:
        case BOTTOM_MIDDLE:
        case BOTTOM_RIGHT:
            // shift pages up
            break;
    }

    switch (index) {
        case TOP_LEFT:
        case BOTTOM_LEFT:
        case LEFT:
            uint32_t indices[] = { 2, 5, 8 };
            pages_swap_horz(world, indices, -1);
            break;
    }

    switch (index) {
        case TOP_RIGHT:
        case BOTTOM_RIGHT:
        case RIGHT:
            uint32_t indices[] = { 0, 3, 6 };
            pages_swap_horz(world, indices, 1);
            break;
    }

    log_debug("Player moved to page %d\n", world->chunk.index);
}
