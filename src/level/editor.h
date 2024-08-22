#ifndef _LEVEL_EDITOR_H_
#define _LEVEL_EDITOR_H_

#include "world/world.h"

typedef enum {
    ES_IDLE,
    ES_MOVE,
    ES_PLACE,
} editor_state_e;

typedef struct {
    editor_state_e state;
    vec2s offset;
    entity_t *entity;
} editor_t;

/* Initializes the level editor. */
void editor_init(editor_t *);

/* Updates the level editor. */
void editor_sync(editor_t *, world_t *);

#endif // ifndef _LEVEL_EDITOR_H_
