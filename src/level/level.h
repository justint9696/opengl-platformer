#ifndef _LEVEL_LEVEL_H_
#define _LEVEL_LEVEL_H_

#include <cglm/types-struct.h>
#include <inttypes.h>
#include <stddef.h>

// corresponding to entities and tiles
typedef struct {
    int type;
    int texture;
    uint32_t color;
    vec2s pos;
    vec2s dim;
} level_data_t;

typedef struct {
    // size in bytes
    size_t size;
   
    // array of level_data_t (entities or tiles)
    level_data_t *data;
} chunk_t;

typedef struct {
    // array of chunks
    chunk_t *chunks;
} level_t;

void level_init(level_t *, const char *fname);
void level_destroy(level_t *);

void level_swap_chunks(level_t *, uint32_t idx);

#endif
