#include "status.h"

#include <cglm/struct.h>

#define CHUNK_SIZE 512

static struct {
    vec2s pos;
    vec2s dim;
} chunk;

static void init() {
    chunk.pos = (vec2s) { -CHUNK_SIZE * 3, -CHUNK_SIZE };
    chunk.dim = (vec2s) { 7, 3 };
}

static inline vec2s glms_vec2_abs(vec2s v) {
    return (vec2s) { .x = fabs(v.x), .y = fabs(v.y) };
}

static int chunk_index_from_pos(vec2s pos) {
    vec2s dim = chunk.dim;
    vec2s coord = glms_vec2_divs(glms_vec2_sub(pos, chunk.pos), CHUNK_SIZE);
    return (coord.x + (dim.y - floorf(coord.y) - 1) * dim.x);
}

static int run_tests_one(vec2s v[], int a[], size_t n) {
    for (size_t i = 0; i < n; i++) {
        vec2s pos = v[i];
        int ans = a[i];
        int index = chunk_index_from_pos(pos);
        if (index != ans) {
            fprintf(stderr, "Error: Test case %ld failed: `%d` != %d.\n",
                    (i + 1), index, ans);
            return -1;
        }
    }
    return 0;
}

#define chunk_loop(_chunk)                                                     \
    __typeof__(_chunk) __chunk = (_chunk);                                     \
    for (int y = 0; y < __chunk.dim.y; y++)                                    \
        for (int x = 0; x < __chunk.dim.x; x++)

static int run_tests_two(int iter) {
    int n = 0;
    int index = 0;
    chunk_loop(chunk) {
        vec2s pos = { -1536 + (x * CHUNK_SIZE), 512 - (y * CHUNK_SIZE) };
        for (int i = 0; i < iter; i++) {
            n++;
            int res = chunk_index_from_pos(pos);
            if (index != res) {
                fprintf(stderr,
                        "Error: Test case %d failed: (%d, %d) at (%.2f, %.2f): "
                        "`%d` != %d.\n",
                        n, x, y, pos.x, pos.y, res, index);
                return -1;
            }
            pos = glms_vec2_adds(pos, floorf(1.f * CHUNK_SIZE / iter));
        }
        index++;
    }

    return 0;
}

#define arraylen(_arr) (sizeof(_arr) / sizeof(_arr[0]))

int main(void) {
    init();
    vec2s v[] = {
        { -1536,    0 },
        { -1280,  256 },
        { -1024,    0 },
        {  -768,  256 },
        {  -512,    0 },
        {     0,    0 },
        {   128,  128 },
        {   256,  256 },
        { -1536, -512 },
        {    -1,  109 },
        {    -1,   87 },
    };

    int a[] = {
        7, 7, 8, 8, 9, 10, 10, 10, 14, 9, 9,
    };

    int ret;
    if ((ret = run_tests_one(v, a, arraylen(v))) == -1) {
        goto done;
    }

    if ((ret = run_tests_two(10)) == -1) {
        goto done;
    }

done:
    log_status("Chunk Index", ret);

    return 0;
}
