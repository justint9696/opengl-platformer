#include "entity/table.h"

#include <assert.h>

#define _DECL_TABLE(_name)\
    extern void *_name##_create(vec2s, vec2s, world_t *);

_DECL_TABLE(enemy);
_DECL_TABLE(platform);

const create_fn_t CREATE_TABLE[ET_MAX] = {
    NULL,
    enemy_create,
    platform_create,
};

inline create_fn_t table_lookup(entity_e idx) {
    assert(idx < ET_MAX);
    return CREATE_TABLE[idx];
}
