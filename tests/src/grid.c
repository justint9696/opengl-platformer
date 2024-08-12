#include <cglm/struct.h>
#include <stdio.h>

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

#define CELL_MAX      32
#define CELL_SIZE     15

#define BUCKET_MAX    ((SCREEN_WIDTH / CELL_SIZE) + (SCREEN_HEIGHT / CELL_SIZE))

#define LOG(_message, ...) ({\
        fprintf(stdout, "[DEBUG][%s:%d]: "\
                _message, __FILE__, __LINE__, ##__VA_ARGS__);\
    })

#define HASH(_x, _y, _n)\
    ((((_x) * 73856093L) ^ ((_y) * 19349663L)) % _n);

#define OFFSET(_x, _y, _size) ((_x) + ((_y) * (_size)))

#define INDEX_OF(_x, _y, _size) ((ivec2s) {\
        .x = ((_x) / _size),\
        .y = ((_y) / _size),\
    })

typedef struct {
    vec2s pos;
    vec2s dim;
} box_t;

typedef struct obj_s {
    union { 
        box_t box; 
        struct {
            vec2s pos;
            vec2s dim; 
        };
    };
    struct obj_s *next;
} obj_t;

typedef struct {
    size_t len;
    void *arr[CELL_MAX];
} cell_t;

typedef struct grid_s {
    cell_t bucket[BUCKET_MAX];
} grid_t;

static grid_t g_grid = { 0 };

static void grid_insert(grid_t *self, vec2s pos, void *data) {
    ivec2s index = INDEX_OF(pos.x, pos.y, CELL_SIZE);
    /* glms_ivec2_print(index, stdout); */
    size_t key = HASH(index.x, index.y, BUCKET_MAX);
    cell_t *cell = &self->bucket[key];
    cell->arr[cell->len++] = data;
    LOG("vertex (%.2f, %.2f) inserted into cell: %ld\n", pos.x, pos.y, key);
}

static void grid_update(grid_t *self) {
    size_t n = BUCKET_MAX;
    for (size_t i = 0; i < n; i++) {
        cell_t *cell = &self->bucket[i];

        if (cell->len < 2)
            continue;

        LOG("potential collision in cell: %ld\n", i);
        for (size_t j = 0; j < cell->len; j++) {
            box_t *box = cell->arr[j];
            LOG("quad at (%.2f, %.2f)\n", box->pos.x, box->pos.y);
        }
    }
}

static inline void box_insert(box_t *box) {
    LOG("inserting vertices into grid...\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            grid_insert(&g_grid, (vec2s) { 
                .x = box->pos.x + (box->dim.x * i),
                .y = box->pos.y + (box->dim.y * j),
            }, box);
        }
    }
}

int main(void) {
    LOG("hello world!\n");
    LOG("bucket len: %d\n", BUCKET_MAX);

    box_t a = {
        .pos = { 10, 10 },
        .dim = { 5, 5 },
    };
    /* box_insert(&a); */
    grid_insert(&g_grid, a.pos, &a);

    box_t b = {
        .pos = { 16, 16 },
        .dim = { 5, 5 },
    };
    /* box_insert(&b); */
    grid_insert(&g_grid, b.pos, &b);

    box_t c = {
        .pos = { 22, 22 },
        .dim = { 5, 5 },
    };
    /* box_insert(&c); */
    grid_insert(&g_grid, c.pos, &c);

    grid_update(&g_grid);

    return 0;
}
