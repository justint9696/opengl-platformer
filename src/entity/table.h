#ifndef _ENTITY_TABLE_H_
#define _ENTITY_TABLE_H_

#include "world/world.h"

#include <stdint.h>

typedef void *(*create_fn_t)(vec2s, vec2s, world_t *);

create_fn_t table_lookup(entity_e);

#endif // _ENTITY_TABLE_H_
