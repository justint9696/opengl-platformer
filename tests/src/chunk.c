#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <cglm/struct.h>

#define XASSERT(_e, _msg, ...) do {\
    if (!(_e)) {\
        fprintf(stderr, "ERROR: %s:%d: " \
                _msg, __FILE__, __LINE__, __VA_ARGS__);\
        exit(1);\
    }\
} while (0);

#define CHUNK_SIZE 384
#define CHUNK_MAX  64

typedef struct {
    int type;
    vec2s pos;
    vec2s dim;
} data_t;

typedef struct {
    size_t n;
    data_t arr[CHUNK_MAX];
} list_t;

typedef struct {
    int index;
    list_t entities;
} page_t;

typedef struct {
    int origin;
    ivec2s dim;
    vec2s spawn;
    page_t pages[9];
} chunk_t;

static struct {
    struct {
        chunk_t chunk;
    } level;
} state;

enum {
    F_ERROR,
    F_SUCCESS,
};

static void level_init() {
    memset(&state.level, 0, sizeof(state.level));

    chunk_t *chunk = &state.level.chunk;
    chunk->origin = 11;
    chunk->dim = (ivec2s) { 7, 3 };
}

static int level_import(const char *fpath) {
    int ret = F_ERROR;
    FILE *fp = fopen(fpath, "rb");
    if (!fp) {
        goto done;
    }

    fseek(fp, 0, SEEK_SET);

    chunk_t *chunk = &state.level.chunk;
    fread(&chunk->origin, sizeof(int), 1, fp);
    fread(&chunk->dim, sizeof(ivec2s), 1, fp);

    data_t data;
    fread(&data, sizeof(data_t), 1, fp);
    printf("player = { type: %d, pos: (%.2f, %.2f), dim: (%.2f, %.2f) }\n",
            data.type, data.pos.x, data.pos.y, data.dim.x, data.dim.y);

    list_t ent;
    for (int i = 0; i < chunk->dim.y; i++) {
        for (int j = 0; j < chunk->dim.x; j++) {
            fread(&ent.n, sizeof(size_t), 1, fp);
            fread(&ent.arr, sizeof(data_t), ent.n, fp);

            printf("chunk (%d, %d) has %ld entities.\n",
                    i, j, ent.n);
            for (size_t i = 0; i < ent.n; i++) {
                data_t *e = &ent.arr[i];
                printf("{ type: %d, pos: (%.2f, %.2f), dim: (%.2f, %.2f) }\n",
                        e->type, e->pos.x, e->pos.y, e->dim.x, e->dim.y);
            }
        }
    }

    ret = F_SUCCESS;

done:
    fclose(fp);
    return ret;
}

/*
 * int      origin;
 * ivec2s   dchunk;
 * vec2s    spawnpoint;
 * for (0 .. nchunk) {
 *     size_t nent;
 *     data_t entities[nent];
 * }
 */
static int level_export(const char *fpath) {
    int ret = F_ERROR;
    FILE *fp = fopen(fpath, "wb+");
    if (!fp) {
        goto done;
    }

    fseek(fp, 0, SEEK_SET);

    // chunk origin
    fwrite(&state.level.chunk.origin, sizeof(int), 1, fp);

    // chunk dimensions
    fwrite(&state.level.chunk.dim, sizeof(ivec2s), 1, fp);

    // chunk player
    data_t player = {
        .type = 0,
        .pos = (vec2s) { 128.f, 128.f },
        .dim = (vec2s) { 50.f, 50.f },
    };
    fwrite(&player, sizeof(data_t), 1, fp);

    // first 5 pages have 0 entities
    size_t count = 0;
    for (int i = 0; i < state.level.chunk.dim.x; i++)
        fwrite(&count, sizeof(size_t), 1, fp);


    float target = -CHUNK_SIZE * 4.f;

    data_t data = (data_t) {
        .type = 2,
        .pos = { target, 25.f },
        .dim = { 50.f, 50.f },
    };

    size_t n;
    data_t arr[24];

    for (int i = 0; i < state.level.chunk.dim.x; i++) {
        n = 0;
        memset(arr, 0, sizeof(data_t) * 24);

        target += CHUNK_SIZE;
        while (data.pos.x <= target && n < 24) {
            memcpy(&arr[n++], &data, sizeof(data_t));
            data.pos.x += 50.f;
        }

        fwrite(&n, sizeof(size_t), 1, fp);
        fwrite(&arr, sizeof(data_t), n, fp);
    }

    /* size_t n = floorf(CHUNK_SIZE / 50.f); */
    /* for (int i = 0; i < 5; i++) { */
    /*     fwrite(&n, sizeof(size_t), 1, fp); */
    /*     for (size_t j = 0; j < n; j++) { */
    /*         fwrite(&data, sizeof(data_t), 1, fp); */
    /*         data.pos.x += 50.f; */
    /*     } */
    /* } */

    // last 5 pages have 0 entities
    for (int i = 0; i < state.level.chunk.dim.x; i++)
        fwrite(&count, sizeof(size_t), 1, fp);

    ret = F_SUCCESS;

done:
    fclose(fp);
    return ret;
}

int main(int argc, char *argv[]) {
    level_init();

    int ret;
    const char *fname = "data/demo.dat";
    if ((ret = level_export(fname)) == F_ERROR) {
        perror(fname);
        return 1;
    }

    printf("file \"%s\" exported successfully!\n", fname);

    if ((ret = level_import(fname)) == F_ERROR) {
        perror(fname);
        return 1;
    }

    printf("file \"%s\" imported successfully!\n", fname);
    return 0;
}
