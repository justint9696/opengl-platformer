#include <cglm/struct.h>
#include <stdio.h>

#define CHUNK_SIZE 512

static struct {
    vec2s pos;
    vec2s dim;
} chunk;

static void init() {
    chunk.pos = (vec2s) { -CHUNK_SIZE * 3, -CHUNK_SIZE };
    chunk.dim = (vec2s) { 7, 3 };
}

static inline vec2s glms_vec2_floor(vec2s v) {
    return (vec2s) {
        .x = floorf(v.x),
        .y = floorf(v.y),
    };
}

/* static inline vec2s glms_vec2_floor(vec2s v) { */
/*     return (vec2s) { */
/*         .x = (v.x > 0.f) ? floorf(v.x) : ceilf(v.x), */
/*         .y = (v.y > 0.f) ? floorf(v.y) : ceilf(v.y) */
/*     }; */
/* } */

static inline vec2s glms_vec2_abs(vec2s v) {
    return (vec2s) { .x = fabs(v.x), .y = fabs(v.y) };
}

static int chunk_index_from_pos(vec2s pos) {
    vec2s coord = glms_vec2_divs(glms_vec2_sub(pos, chunk.pos), CHUNK_SIZE);
    glms_vec2_print(coord, stdout);
    return fabsf(floorf(coord.x)
                 + ((chunk.dim.y - fabsf(floorf(coord.y)) - 1) * chunk.dim.x));
}

static void run_tests_one(vec2s v[], int a[], size_t n) {
    for (size_t i = 0; i < n; i++) {
        vec2s pos = v[i];
        int ans = a[i];
        int index = chunk_index_from_pos(pos);
        if (index != ans) {
            fprintf(stderr, "Error: Test case %ld failed: `%d` != %d.\n",
                    (i + 1), index, ans);
            exit(EXIT_FAILURE);
        } else {
        }
    }
}

#define chunk_loop(_chunk)                                                     \
    __typeof__(_chunk) __chunk = (_chunk);                                     \
    for (int y = 0; y < __chunk.dim.y; y++)                                    \
        for (int x = 0; x < __chunk.dim.x; x++)

static void run_tests_two() {
    int n = 0;
    int index = 0;
    int max_iter = 2;
    chunk_loop(chunk) {
        vec2s pos = { -1536 + (x * CHUNK_SIZE), 512 - (y * CHUNK_SIZE) };
        for (int i = 0; i < max_iter; i++) {
            n++;
            int res = chunk_index_from_pos(pos);
            if (index != res) {
                fprintf(stderr,
                        "Error: Test case %d failed: (%d, %d) at (%.2f, %.2f): "
                        "`%d` != %d.\n",
                        n, x, y, pos.x, pos.y, res, index);
                exit(EXIT_FAILURE);
            } else {
                printf("Success: Test case %d: (%.2f, %.2f) is in index %d.\n",
                       n, pos.x, pos.y, index);
            }

            pos = glms_vec2_adds(pos, floorf(1.f * CHUNK_SIZE / max_iter));
        }
        index++;
    }
}

#define arraylen(_arr) (sizeof(a) / sizeof(a[0]))

int main(void) {
    init();
    vec2s v[] = {
        { -1536,     0 },
        { -1280,   256 },
        { -1024,     0 },
        {  -768,   256 },
        {  -512,     0 },
        {     0,     0 },
        {   128,   128 },
        {   256,   256 },
        { -1536, -512 },
        { -1, 109 },
    };

    int a[] = {
        7, 7, 8, 8, 9, 10, 10, 10, 14, 9,
    };
    run_tests_one(v, a, arraylen(v));
    run_tests_two();
    printf("All test cases passed!\n");
    return 0;
}
