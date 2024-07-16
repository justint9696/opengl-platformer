#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cglm/struct.h>

void _log_message(FILE *stream, const char *title, const char *file, int line,
                  const char *format, ...) {
    va_list args;
    va_start(args, format);

    char text[1024];
    vsnprintf(text, 1024, format, args);
    fprintf(stream, "%s: %s:%d: %s", title, file, line, text);

    va_end(args);
}

#define XASSERT(_e, _msg, ...) do {\
    if (!(_e)) {\
        fprintf(stderr, "ERROR: %s:%d: " \
                _msg, __FILE__, __LINE__, __VA_ARGS__);\
        exit(1);\
    }\
} while (0);

#define log_and_fail(...) do {\
    _log_message(stderr, "ERROR", __FILE__, __LINE__, __VA_ARGS__);\
    exit(1);\
} while (0);

#define log_debug(...)\
    _log_message(stderr, "DEBUG", __FILE__, __LINE__, __VA_ARGS__);

#define CHUNK_SIZE 256
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
    chunk->origin = 8;
    chunk->dim = (ivec2s) { 5, 3 };
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

    fwrite(&state.level.chunk.origin, sizeof(int), 1, fp);
    fwrite(&state.level.chunk.dim, sizeof(ivec2s), 1, fp);

    data_t player = {
        .type = 0,
        .pos = (vec2s) { 128.f, 128.f },
        .dim = (vec2s) { 50.f, 50.f },
    };
    fwrite(&player, sizeof(data_t), 1, fp);

    size_t count = 0;
    for (int i = 0; i < 5; i++)
        fwrite(&count, sizeof(size_t), 1, fp);

    data_t data = (data_t) {
        .type = 2,
        .pos = { -CHUNK_SIZE, 25.f },
        .dim = { 50.f, 50.f },
    };

    size_t n = 5;
    for (int i = 0; i < 5; i++) {
        fwrite(&n, sizeof(size_t), 1, fp);
        for (size_t j = 0; j < n; j++) {
            fwrite(&data, sizeof(data_t), 1, fp);
            data.pos.x += 50.f;
        }
    }

    for (int i = 0; i < 5; i++)
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

    log_debug("file \"%s\" exported successfully!\n", fname);

    if ((ret = level_import(fname)) == F_ERROR) {
        perror(fname);
        return 1;
    }

    log_debug("file \"%s\" imported successfully!\n", fname);
    return 0;
}
