#ifndef _LEVEL_EDITOR_H_
#define _LEVEL_EDITOR_H_

#include "world/world.h"

typedef struct {
    int current_tile;
    int curent_entity;
} level_editor_t;

void level_editor_init(level_editor_t *);
void level_editor_update(level_editor_t *, world_t *);

#endif
