/**
 * @file editor.h
 * @author Justin Tonkinson
 * @date 2024/06/10
 * @brief Level editor data structures and function prototypes.
 */

#ifndef _LEVEL_EDITOR_H_
#define _LEVEL_EDITOR_H_

#include "data/fsm.h"
#include "util/aabb.h"
#include "world/world.h"

#include <cglm/types-struct.h>
#include <glad/glad.h>

/** @brief All possible level editor states. */
typedef enum editor_state_e {
    ES_IDLE,
    ES_EDIT,
    ES_MOVE,
    ES_PLACE,
    ES_SELECT,
    ES_HIGHLIGHT,
    ES_CAMERA,
    ES_MAX,
} editor_state_t;

/** @brief Level editor data structure. */
typedef struct editor_s {
    /** @brief Boolean variable that enables grid snapping when placing. */
    bool snap;

    /** @brief Editor state manager. */
    fsm_t fsm;

    /** @brief Offset from the mouse world position to entity position. */
    vec2s offset;

    /** @brief The currently selected entity. */
    entity_t(*entity);

    union {
        /** @brief The bounding box of the selection rectangle. */
        box_t select;

        /** @brief Coordinates for calculating camera movement. */
        struct {
            vec2s start, end;
        };
    };

    /** @brief Vertex buffer objects. */
    GLuint vao, vbo, ibo;
} editor_t;

/** 
 * @brief Initializes the level editor. 
 * @param self a pointer to the level editor
 */
void editor_init(editor_t *);

/** 
 * @brief Releases the memory allocated for the level editor. 
 * @param self a pointer to the level editor
 */
void editor_destroy(editor_t *);

/**
 * @brief Synchronizes the level editor with the tick rate. 
 * @param self a pointer to the level editor
 * @param world a pointer to the game world
 */
void editor_sync(editor_t *, world_t *);

/**
 * @brief Renders the components of the level editor. 
 * @param self a pointer to the level editor
 * @param world a pointer to the game world
 */
void editor_render(editor_t *, world_t *);

#endif // ifndef _LEVEL_EDITOR_H_
