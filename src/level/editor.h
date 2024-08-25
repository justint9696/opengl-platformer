#ifndef _LEVEL_EDITOR_H_
#define _LEVEL_EDITOR_H_

#include "data/fsm.h"
#include "world/world.h"

#include <cglm/types-struct.h>
#include <glad/glad.h>

typedef enum {
    ES_IDLE,
    ES_EDIT,
    ES_MOVE,
    ES_PLACE,
    ES_SELECT,
    ES_CAMERA,
    ES_MAX,
} editor_state_e;

typedef struct {
    // editor state managner
    fsm_t fsm;

    // offset from mouse world pos to entity pos
    vec2s offset;

    // previous position of entity
    vec2s position;

    // current entity
    entity_t *entity;

    // vertex buffer objects
    GLuint vao, vbo, ibo;
} editor_t;

/* Initializes the level editor. */
void editor_init(editor_t *);

/* Releases the memory allocated for the level editor. */
void editor_destroy(editor_t *);

/* Updates the level editor. */
void editor_sync(editor_t *, world_t *);

/* Renders the components of the level editor. */
void editor_render(editor_t *, world_t *);

#endif // ifndef _LEVEL_EDITOR_H_
