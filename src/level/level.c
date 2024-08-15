#include "level/level.h"

#include "data/array.h"
#include "entity/table.h"
#include "util/assert.h"
#include "player.h"

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

        page_t *tmp = chunk_page_from_pos(&world->chunk, data->pos);
        if (tmp->index != page->index) {
            n++;
        }

        create_fn_t fn = table_lookup(data->type);
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
    level.fp = fopen(fpath, "rb");
    XASSERT(level.fp, "Could not open file `%s`: %s.\n", fpath,
            strerror(errno));

    fseek(level.fp, 0, SEEK_SET);
    fread(&world->chunk.index, sizeof(int), 1, level.fp);
    fread(&world->chunk.dim, sizeof(ivec2s), 1, level.fp);

    // FIXME: calculate the actual position based on the chunk index
    world->chunk.pos = glms_vec2_fill(-CHUNK_SIZE);

    ldata_t data;
    fread(&data, sizeof(ldata_t), 1, level.fp);

    world->player = player_init(data.pos, data.dim, world);

    struct {
        size_t n;
        ldata_t arr[CHUNK_MAX];
    } entities;

    int32_t size = (world->chunk.dim.x * world->chunk.dim.y);
    level.offsets = array_alloc(sizeof(int32_t), size);

    // current page index
    uint32_t index = 0;

    // top left page of starting chunk
    int32_t target = glm_max(0, world->chunk.index - world->chunk.dim.x - 1);

    for (int32_t i = 0; i < size; i++) {
        // offset within level file
        uint32_t offset = ftell(level.fp);
        array_push(level.offsets, &offset);

        fread(&entities.n, sizeof(size_t), 1, level.fp);
        fread(&entities.arr, sizeof(ldata_t), entities.n, level.fp);

        // have all the pages been loaded or is the page not part of the chunk
        if ((index >= 9) || ((target - i) > 0))
            continue;

        // have all 3 pages been loaded
        if ((i - target) >= 2)
            target = world->chunk.dim.x + target;

        // load pages into memory
        log_debug("Loading index %d into memory\n", i);
        page_t *page = &world->chunk.pages[index++];
        page_calculate_aabb(page, world);
        page_load_data(page, world, entities.arr, entities.n);
    }
}

/* Shifts the provided page indicies around the iterator. */
static void pages_shift(world_t *world, uint32_t indices[], int it) {
    // iterate through all the indices
    for (int i = 0; i < 3; i++) {
        uint32_t index = indices[i];
        page_t *page = &world->chunk.pages[index];

        // remove entities from the world
        size_t len = array_len(page->entities);
        log_debug("Releasing %ld entities from page %d\n", len, page->index);
        for (size_t n = 0; n < len; n++) {
            entity_t *entity = &page->entities[n];
            entity_destroy(entity, world);
        }

        // mark page block as free
        chunk_release_page(&world->chunk, page);

        // shift the pages around the iterator
        for (int j = 1; j < 3; j++) {
            indices[i] += it;
            index = indices[i];

            page_t *tmp = &world->chunk.pages[index];

            page->entities = tmp->entities;
            memcpy(&page->box, &tmp->box, sizeof(box_t));

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

        // calculate the offset from the center chunk
        vec2s coord = glms_vec2_subs(
            glms_vec2_divs(glms_vec2_sub(page->pos, world->chunk.pos),
                           CHUNK_SIZE),
            1.f);

        // calculate the offset of the center chunk with respects to the chunk
        // dimensions
        vec2s idx = (vec2s) {
            .x = world->chunk.index % world->chunk.dim.x,
            .y = floorf(1.f * world->chunk.index / world->chunk.dim.x),
        };

        idx = glms_vec2_add(idx, coord);

        // this allows (-1, -1) to be the top left page
        // instead of the bottom left page.
        idx.y = world->chunk.dim.y - idx.y - 1;
        /* log_debug("%.2f, %.2f\n", idx.x, idx.y); */

        if ((idx.x < 0 || idx.x >= world->chunk.dim.x)
            || (idx.y < 0 || idx.y > world->chunk.dim.y))
            continue;

        int32_t index = ((floorf(idx.y) * world->chunk.dim.x) + floorf(idx.x));

        log_debug("Requesting index %u for page %u\n", index, page->index);
        uint32_t offset = *(uint32_t *)array_get(level.offsets, index);
        fseek(level.fp, offset, SEEK_SET);

        fread(&entities.n, sizeof(size_t), 1, level.fp);
        fread(&entities.arr, sizeof(ldata_t), entities.n, level.fp);

        log_debug("Loading %lu entities into page %u\n", entities.n,
                  page->index);

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
            log_debug("Shifting pages right\n");

            uint32_t indices[] = { 2, 5, 8 };
            pages_swap_horz(world, indices, -1);
            break;
    }

    switch (index) {
        case TOP_RIGHT:
        case BOTTOM_RIGHT:
        case RIGHT:
            log_debug("Shifting pages left\n");

            uint32_t indices[] = { 0, 3, 6 };
            pages_swap_horz(world, indices, 1);
            break;
    }

    log_debug("Player moved to page %u\n", world->chunk.index);
}
