#ifndef _LEVEL_EDITOR_H_
#define _LEVEL_EDITOR_H_

#include "game/camera.h"
#include "world/world.h"

typedef struct {
    int current_tile;
    int curent_entity;
} level_editor_t;

void level_editor_init();
void level_editor_destroy();
void level_editor_update(world_t *, const camera_t *);

#endif
