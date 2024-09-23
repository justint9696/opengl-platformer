/**
 * @file level.c
 * @author Justin Tonkinson
 * @date 2024/06/10
 * @brief Level implementation functions.
 * @bug After extending the level bounds, exporting the level occasionally
 * crashes the game.
 */

#include "level/level.h"

#include "data/array.h"
#include "entity/table.h"
#include "player.h"
#include "util/assert.h"
#include "util/log.h"

#include <assert.h>
#include <cglm/struct.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/** @brief Internal level state. */
static level_t level;

/** @brief The position of the page with respects to the center. */
enum {
    BOTTOM_LEFT,
    BOTTOM_MIDDLE,
    BOTTOM_RIGHT,
    LEFT,
    CENTER,
    RIGHT,
    TOP_LEFT,
    TOP_MIDDLE,
    TOP_RIGHT,
};

/**
 * @brief Creates the page bounds based on the chunk position and page index.
 */
static void page_calculate_aabb(page_t *page, world_t *world) {
    page->pos = glms_vec2_add(
        (vec2s) {
            .x = (page->index % CHUNK_WIDTH) * CHUNK_SIZE,
            .y = (2 - floorf((float)page->index / CHUNK_WIDTH)) * CHUNK_SIZE,
        },
        world->chunk.pos);
    page->dim = glms_vec2_fill(CHUNK_SIZE);
}

/** @brief Loads level data into the provided page. */
static void page_load_data(page_t *page, world_t *world, ldata_t arr[],
                           size_t len) {
    /* log_debug("Loading %ld entities into page %d\n", len, page->index); */
    for (size_t i = 0; i < len; i++) {
        ldata_t *data = &arr[i];

        create_fn_t fn = table_lookup(data->type);
        assert(fn);

        entity_t *e = fn(data->pos, data->dim, world);
        XASSERT(e->body.page->index == page->index,
                "Entity at (%.2f, %.2f) was loaded into the wrong page; `%d` "
                "!= %d.\n",
                data->pos.x, data->pos.y, e->body.page->index, page->index);
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
    XASSERT(level.fp, "Failed to create temp file: %s\n", strerror(errno));

    fseek(fp, 0, SEEK_SET);

    fread(&world->chunk.index, sizeof(int), 1, fp);
    fread(&world->chunk.box, sizeof(box_t), 1, fp);

    vec2s idx = (vec2s) {
        .x = fmodf(world->chunk.index, world->chunk.box.dim.x) - 1,
        .y = floorf(1.f * world->chunk.index / world->chunk.box.dim.x) + 1,
    };

    vec2s diff = glms_vec2_negate(glms_vec2_sub(
        (vec2s) { .x = 0.f, .y = world->chunk.box.dim.y - 1 }, idx));

    // calculate the relative position of the current chunk
    world->chunk.pos = glms_vec2_add(world->chunk.box.pos,
                                     glms_vec2_scale(diff, CHUNK_SIZE));

    for (int i = 0; i < 9; i++) {
        page_t *page = &world->chunk.pages[i];
        page_calculate_aabb(page, world);
    }

    ldata_t player;
    fread(&player, sizeof(ldata_t), 1, fp);

    world->player = player_init(player.pos, player.box.dim, world);

    int size = (world->chunk.box.dim.x * world->chunk.box.dim.y);
    level.offsets = array_alloc(sizeof(uint64_t), 256);

    // current page index
    int index = 0;

    // top left page of starting chunk
    int target = glm_max(0, world->chunk.index - world->chunk.box.dim.x - 1);

    size_t len;
    ldata_t data[CHUNK_MAX];
    for (int i = 0; i < size; i++) {
        // offset within level file
        uint64_t offset = ftell(level.fp);
        array_push(level.offsets, &offset);

        // read data from level file
        fread(&len, sizeof(size_t), 1, fp);
        fread(&data, sizeof(ldata_t), len, fp);

        // write data to temporary file with padding
        fwrite(&len, sizeof(size_t), 1, level.fp);
        fwrite(&data, sizeof(ldata_t), CHUNK_MAX, level.fp);

        // have all the pages been loaded or is the page not part of the chunk
        if ((index >= 9) || ((target - i) > 0))
            continue;

        // have all 3 pages been loaded
        if ((i - target) >= 2)
            target = world->chunk.box.dim.x + target;

        /* log_debug("Loading index %d into memory\n", i); */

        // load pages into memory
        page_t *page = &world->chunk.pages[index++];
        page_load_data(page, world, data, len);
    }

    fclose(fp);
}

void level_export(world_t *world, const char *fpath) {
    assert(level.fp);

    FILE *fp = fopen(fpath, "wb+");
    XASSERT(fp, "Could not open file `%s`: %s\n.", fpath, strerror(errno));

    fseek(fp, 0, SEEK_SET);

    world->chunk.index
        = chunk_index_from_pos(&world->chunk, world->player->body.pos);
    fwrite(&world->chunk.index, sizeof(int), 1, fp);
    fwrite(&world->chunk.box, sizeof(box_t), 1, fp);

    fwrite(
        &(ldata_t) {
            .type = world->player->type,
            .box = world->player->body.box,
        },
        sizeof(ldata_t), 1, fp);

    size_t len;
    ldata_t data[CHUNK_MAX];

    for (int i = 0; i < 9; i++) {
        page_t *page = &world->chunk.pages[i];
        len = array_len(page->entities);
        for (size_t n = 0; n < len; n++) {
            entity_t *entity = &page->entities[n];
            data[n] = (ldata_t) {
                .type = entity->type,
                .box = entity->body.box,
            };
        }

        // set file pointer to offset within file
        int index = chunk_index_from_pos(&world->chunk, page->pos);
        uint64_t offset = level.offsets[index];
        fseek(level.fp, offset, SEEK_SET);

        // write primary pages to temp file
        fwrite(&len, sizeof(size_t), 1, level.fp);
        fwrite(&data, sizeof(ldata_t), CHUNK_MAX, level.fp);
    }

    size_t n = array_len(level.offsets);
    for (size_t i = 0; i < n; i++) {
        memset(&data, 0, sizeof(ldata_t) * CHUNK_MAX);

        // set file pointer to offset within file
        uint64_t offset = level.offsets[i];
        fseek(level.fp, offset, SEEK_SET);

        // read data at temp file offset
        fread(&len, sizeof(size_t), 1, level.fp);
        fread(&data, sizeof(ldata_t), len, level.fp);

        // write data to export file without padding
        fwrite(&len, sizeof(size_t), 1, fp);
        fwrite(&data, sizeof(ldata_t), len, fp);
    }

    fclose(fp);
    log_debug("Level exported to file `%s`\n", fpath);
}

/** @brief Shifts the provided page indicies around the iterator. */
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
            // index 0 because as entities are destroyed, their indices offset
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

/** @brief Replaces the provided page indicies around the iterator. */
static void pages_replace(world_t *world, uint32_t indices[], int it) {
    size_t len;
    ldata_t data[CHUNK_MAX];

    // iterate through all the indices
    for (int i = 0; i < 3; i++) {
        page_t *page = &world->chunk.pages[indices[i]];

        // request block of memory
        page->entities = chunk_request_page(&world->chunk, page);
        page_calculate_aabb(page, world);

        int index = chunk_index_from_pos(&world->chunk, page->pos);
        /* glms_vec2_print(page->pos, stdout); */

        log_debug("Requesting index %d for page %d\n", index, page->index);
        uint64_t offset = level.offsets[index];
        fseek(level.fp, offset, SEEK_SET);

        fread(&len, sizeof(size_t), 1, level.fp);
        fread(&data, sizeof(ldata_t), len, level.fp);

        log_debug("Loading %ld entities into page %d\n", len, page->index);
        page_load_data(page, world, data, len);
    }
}

/** @brief Swaps the provided page indicies horizontally around the iterator. */
static void pages_swap_horz(world_t *world, uint32_t indices[], int it) {
    pages_shift(world, indices, it);

    world->chunk.index += it;
    world->chunk.pos.x += (it * CHUNK_SIZE);

    pages_replace(world, indices, it);
}

/** @brief Swaps the provided page indicies vertically around the iterator. */
static void pages_swap_vert(world_t *world, uint32_t indices[], int it) {
    pages_shift(world, indices, it);

    world->chunk.index += (world->chunk.box.dim.x * ((it < 0) ? -1 : 1));
    world->chunk.pos.y += (CHUNK_SIZE * ((it > 0) ? -1 : 1));

    pages_replace(world, indices, it);
}

void level_swap_pages(world_t *world, page_t *page) {
    assert(level.fp);
    uint32_t index = page->index;

    switch (index) {
        case TOP_LEFT:
        case TOP_MIDDLE:
        case TOP_RIGHT:
            // shift pages down
            pages_swap_vert(world, (uint32_t[]) { 6, 7, 8 }, -3);
            break;
    }

    switch (index) {
        case BOTTOM_LEFT:
        case BOTTOM_MIDDLE:
        case BOTTOM_RIGHT:
            // shift pages up
            pages_swap_vert(world, (uint32_t[]) { 0, 1, 2 }, 3);
            break;
    }

    switch (index) {
        case TOP_LEFT:
        case BOTTOM_LEFT:
        case LEFT:
            pages_swap_horz(world, (uint32_t[]) { 2, 5, 8 }, -1);
            break;
    }

    switch (index) {
        case TOP_RIGHT:
        case BOTTOM_RIGHT:
        case RIGHT:
            pages_swap_horz(world, (uint32_t[]) { 0, 3, 6 }, 1);
            break;
    }
}

/** @brief Extends the bounds of the level within the temporary level file */
static void extend_bounds(world_t *world, uint32_t start, size_t n, int it) {
    size_t len = 0;
    ldata_t data[CHUNK_MAX] = { 0 };

    uint32_t index = start;
    for (size_t i = 0; i < n; i++) {
        // move temp file pointer to the end of the file
        fseek(level.fp, 0, SEEK_END);

        // insert temp file offset into the offsets array at the current index
        uint64_t offset = ftell(level.fp);
        log_debug("Inserting offset %ld into index %d\n", offset, index);
        array_insert(level.offsets, index, &offset);

        // write empty data to temp file with padding
        fwrite(&len, sizeof(size_t), 1, level.fp);
        fwrite(&data, sizeof(ldata_t), CHUNK_MAX, level.fp);

        index += it;
    }
}

static void level_extend_bounds(world_t *world, page_t *page, int index) {
    bool case1 = fmodf(index, world->chunk.box.dim.x) == 0;
    bool case2
        = fmodf(index, world->chunk.box.dim.x) == (world->chunk.box.dim.x - 1);
    bool case3 = floorf(index / world->chunk.box.dim.x) == 0;
    bool case4 = floorf(index / world->chunk.box.dim.x)
                 == (world->chunk.box.dim.y - 1);

    /* log_debug("Index: %d Coords: (%.0f, %.0f)\n", index, */
    /*           fmodf(index, world->chunk.box.dim.x), */
    /*           floorf(index / world->chunk.box.dim.x)); */
    /* log_debug("%d: %d: %d: %d\n", case1, case2, case3, case4); */

    if (case1 || case2 || case3 || case4) {
        index = page->index;

        switch (index) {
            case TOP_LEFT:
            case TOP_MIDDLE:
            case TOP_RIGHT:
                // extend the bounds upward
                size_t n = world->chunk.box.dim.x;
                world->chunk.box.dim.y++;
                world->chunk.index += world->chunk.box.dim.x;
                extend_bounds(world, 0, n, 1);
                break;
        }

        switch (index) {
            case BOTTOM_LEFT:
            case BOTTOM_MIDDLE:
            case BOTTOM_RIGHT:
                // extend the bounds downward
                size_t n = world->chunk.box.dim.x;
                int start = n * (++world->chunk.box.dim.y - 1);
                world->chunk.box.pos.y -= CHUNK_SIZE;
                extend_bounds(world, start, n, 1);
                break;
        }

        switch (index) {
            case TOP_LEFT:
            case BOTTOM_LEFT:
            case LEFT:
                // extend the bounds left
                world->chunk.index
                    += floorf(world->chunk.index / world->chunk.box.dim.x) + 1;
                world->chunk.box.pos.x -= CHUNK_SIZE;
                int it = ++world->chunk.box.dim.x;
                size_t n = world->chunk.box.dim.y;
                extend_bounds(world, 0, n, it);
                break;
        }

        switch (index) {
            case TOP_RIGHT:
            case BOTTOM_RIGHT:
            case RIGHT:
                // extend the bounds right
                world->chunk.index
                    += floorf(world->chunk.index / world->chunk.box.dim.x);
                int it = ++world->chunk.box.dim.x;
                size_t n = world->chunk.box.dim.y;
                extend_bounds(world, it - 1, n, it);
                break;
        }

        log_debug("Extended level bounds: (%.2f, %.2f) %d\n",
                  world->chunk.box.dim.x, world->chunk.box.dim.y,
                  world->chunk.index);
    }
}

void level_sync(world_t *world, float dt) {
    // index 4 is the center page
    page_t *page = &world->chunk.pages[4];
    vec2s pos = glms_vec2_negate(glms_vec2(world->camera.pos));

    if (aabb_intersect_2d(&page->box, pos))
        return;

    int index = chunk_index_from_pos(&world->chunk, pos);

    log_debug("tmp->index: \n");
    page_t *tmp = chunk_page_from_pos(&world->chunk, pos);
    assert(tmp);

    level_extend_bounds(world, tmp, index);
    level_swap_pages(world, tmp);

    log_debug("Camera moved to page %d: %d.\n", world->chunk.index, index);
}
