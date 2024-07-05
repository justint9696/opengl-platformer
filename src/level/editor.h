#ifndef _LEVEL_EDITOR_H_
#define _LEVEL_EDITOR_H_

#include "world/world.h"

typedef struct {
    int current_tile;
    int curent_entity;
} leveleditor_t;

void level_editor_init(leveleditor_t *);
void level_editor_update(leveleditor_t *, world_t *);

#endif
