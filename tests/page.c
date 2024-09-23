#include "status.h"

#include <cglm/struct.h>
#include <string.h>

#define CHUNK_SIZE 512

typedef struct page_s {
    int index;
    vec2s pos, dim;
} page_t;

static struct {
    vec2s pos;
    page_t pages[9];
} chunk;

static void init() {
    int n = CHUNK_SIZE;
    page_t pages[9] = {
        { .index = 0, .pos = { -n,  n }, },
        { .index = 1, .pos = {  0,  n }, },
        { .index = 2, .pos = {  n,  n }, },
        { .index = 3, .pos = { -n,  0 }, },
        { .index = 4, .pos = {  0,  0 }, },
        { .index = 5, .pos = {  n,  0 }, },
        { .index = 6, .pos = { -n, -n }, },
        { .index = 7, .pos = {  0, -n }, },
        { .index = 8, .pos = {  n, -n }, },
    };

    for (int i = 0; i < 9; i++) {
        pages[i].dim = (vec2s) { n, n };
    }

    chunk.pos = pages[6].pos;
    memcpy(&chunk.pages, &pages, sizeof(page_t) * 9);
}

static page_t *chunk_page_from_pos(vec2s pos) {
    vec2s coord = glms_vec2_divs(glms_vec2_sub(pos, chunk.pos), CHUNK_SIZE);
    int index = (floorf(coord.x) + ((2 - floorf(coord.y)) * 3));
    assert(index >= 0 && index < 9);
    return &chunk.pages[index];
}

static int run_tests(int iter) {
    int n = 0;
    for (int i = 0; i < 9; i++) {
        const page_t *page = &chunk.pages[i];
        vec2s pos = page->pos;
        for (int j = 0; j < iter; j++) {
            page_t *tmp = chunk_page_from_pos(pos);
            if (tmp->index != page->index) {
                fprintf(
                    stderr,
                    "Error: Test case %d failed: (%.2f, %.2f): `%d` != %d\n", n,
                    pos.x, pos.y, tmp->index, page->index);
                return -1;
            }
            pos = glms_vec2_adds(pos, floorf(1.f * CHUNK_SIZE) / iter);
        }
    }
    return 0;
}

int main(void) {
    init();

    int ret;
    if ((ret = run_tests(10)) == -1) {
        goto done;
    }

done:
    log_status("Chunk Page", ret);
    return 0;
}
