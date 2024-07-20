#ifndef _LEVEL_EDITOR_H_
#define _LEVEL_EDITOR_H_

#include "world/world.h"

typedef struct {
    int current_tile;
    int curent_entity;
} editor_t;

void editor_init(editor_t *);
void editor_update(editor_t *, world_t *);

#endif // _LEVEL_EDITOR_H_
