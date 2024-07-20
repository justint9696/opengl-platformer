#ifndef _LEVEL_LEVEL_H_
#define _LEVEL_LEVEL_H_

#include "world/world.h"

#include <cglm/types-struct.h>

void level_import(world_t *, const char *fpath);
void level_export(const world_t *, const char *fpath);
void level_shutdown();

void level_swap_chunks(world_t *, int index);

#endif // _LEVEL_LEVEL_H_
