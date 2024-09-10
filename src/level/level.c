/**
 * @file level.c
 * @author Justin Tonkinson
 * @date 2024/06/10
 * @brief Level implementation functions.
 * @bug Entities are being spawn into the "wrong page" because of how the level
 * is being exported. Look into the primary page upon export (which depends on
 * the player's location at the time of export).
 * @bug Since the pages are now being swapped around the camera's position and
 * the player exists independently from the world, the game is crashing because
 * it is trying to modify the grid while the player is off screen.
 * @bug After extending the level bounds, exporting the level occasionally
 * crashes the game.
 */

#include "level/level.h"

#include "data/array.h"
#include "entity/table.h"
#include "player.h"
#include "util/assert.h"

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
    size_t n = 0;

    log_debug("Loading %ld entities into page %d\n", len, page->index);
    for (size_t i = 0; i < len; i++) {
        ldata_t *data = &arr[i];

        page_t *tmp = chunk_page_from_pos(&world->chunk, data->pos);
        assert(tmp);

        if (tmp->index != page->index) {
            n++;
            log_debug("Entity at (%.2f, %.2f) was loaded into the wrong page; "
                      "page %d instead of page %d.\n",
                      data->pos.x, data->pos.y, tmp->index, page->index);
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
    XASSERT(level.fp, "Failed to create temp file: %s\n", strerror(errno));

    fseek(fp, 0, SEEK_SET);
    fread(&world->chunk.index, sizeof(int), 1, fp);
    fread(&world->chunk.dim, sizeof(ivec2s), 1, fp);

    // FIXME: calculate the actual position based on the chunk index
    world->chunk.pos = glms_vec2_fill(-CHUNK_SIZE);

    ldata_t player;
    fread(&player, sizeof(ldata_t), 1, fp);
    fwrite(&player, sizeof(ldata_t), 1, level.fp);

    world->player = player_init(player.pos, player.dim, world);

    int size = (world->chunk.dim.x * world->chunk.dim.y);
    level.offsets = array_alloc(sizeof(uint64_t), 128);

    // current page index
    int index = 0;

    // top left page of starting chunk
    int target = glm_max(0, world->chunk.index - world->chunk.dim.x - 1);

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
            target = world->chunk.dim.x + target;

        /* log_debug("Loading index %d into memory\n", i); */

        // load pages into memory
        page_t *page = &world->chunk.pages[index++];
        page_calculate_aabb(page, world);
        page_load_data(page, world, data, len);
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
        if (index == -1)
            continue;

        log_debug("Requesting index %d for page %d\n", index, page->index);
        uint64_t offset = level.offsets[index];
        fseek(level.fp, offset, SEEK_SET);

        fread(&len, sizeof(size_t), 1, level.fp);
        fread(&data, sizeof(ldata_t), len, level.fp);

        /* log_debug("Loading %ld entities into page %d\n", len, */
        /*           page->index); */

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

// TODO: implement swapping pages above or below the current page
void level_swap_pages(world_t *world, page_t *page) {
    assert(level.fp);

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
static void level_extend_bounds(world_t *world, uint32_t start, size_t n,
                                int it) {
    size_t len = 0;
    ldata_t data[CHUNK_MAX] = { 0 };

    uint32_t index = start;
    for (size_t i = 0; i < n; i++) {
        // move temp file pointer to the end of the file
        fseek(level.fp, 0, SEEK_END);

        // insert temp file offset into the offsets array at the current index
        uint64_t offset = ftell(level.fp);
        /* log_debug("index %d offset %ld len %ld\n", index, offset, */
        /*           array_len(level.offsets)); */
        array_insert(level.offsets, index, &offset);
        assert(level.offsets[index] == offset);

        // write empty data to temp file with padding
        fwrite(&len, sizeof(size_t), 1, level.fp);
        fwrite(&data, sizeof(ldata_t), CHUNK_MAX, level.fp);

        index += it;
    }
}

void level_sync(world_t *world, float dt) {
    // index 4 is the center page
    page_t *page = &world->chunk.pages[4];
    vec2s pos = { -world->camera.pos.x, -world->camera.pos.y };

    if (aabb_intersect_2d(&page->box, pos))
        return;

    int index = chunk_index_from_pos(&world->chunk, pos);
    page_t *tmp = chunk_page_from_pos(&world->chunk, pos);
    if ((index == -1) || (index % world->chunk.dim.x == 0)) {
        index = tmp->index;

        switch (index) {
            case TOP_LEFT:
            case TOP_MIDDLE:
            case TOP_RIGHT:
                // extend the bounds upward
                break;
        }

        switch (index) {
            case BOTTOM_LEFT:
            case BOTTOM_MIDDLE:
            case BOTTOM_RIGHT:
                // extend the bounds downward
                break;
        }

        switch (index) {
            case TOP_LEFT:
            case BOTTOM_LEFT:
            case LEFT:
                // extend the bounds left
                world->chunk.index
                    += floorf(1.f * world->chunk.index / world->chunk.dim.x)
                       + 1;
                int it = ++world->chunk.dim.x;
                size_t n = world->chunk.dim.y;
                level_extend_bounds(world, 0, n, it);
                break;
        }

        switch (index) {
            case TOP_RIGHT:
            case BOTTOM_RIGHT:
            case RIGHT:
                // extend the bounds right
                world->chunk.index
                    += floorf(1.f * world->chunk.index / world->chunk.dim.x);
                int it = ++world->chunk.dim.x;
                size_t n = world->chunk.dim.y;
                level_extend_bounds(world, n, n, it);
                break;
        }

        log_debug("Extended level bounds: (%d, %d) %d\n", world->chunk.dim.x,
                  world->chunk.dim.y, world->chunk.index);
    }

    level_swap_pages(world, tmp);
    log_debug("Camera moved to page %d.\n", world->chunk.index);
}
