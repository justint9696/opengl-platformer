#ifndef _ENTITY_ENTITY_H_
#define _ENTITY_ENTITY_H_

#include "game/types.h"

#include <stdint.h>

struct world_s;

typedef enum {
    // should the entity respond to player input
    EF_PLAYER_CONTROLLED = 1 << 0, 

    // should the entity controlled by an ai
    EF_AI_CONTROLLED     = 1 << 1, 

    // should the entity be expected to move
    EF_KINEMATIC         = 1 << 2, 

    // should the entity respond to gravity
    EF_GRAVITY           = 1 << 3, 
                                  
    // should the camera follow the entity's movement
    EF_CAMERA_FOLLOW     = 1 << 4,

    EF_NONE              = 0,
} flag_e;

typedef enum {
    ET_PLAYER,
    ET_ENEMY,
    ET_PLATFORM,
    ET_MAX,
} entity_e;

typedef struct entity_s {
    int32_t id;
    int32_t flags;
    int32_t buttons;
    entity_e type;
    body_t body;

    void (*init)(struct entity_s *, struct world_s *);
    void (*destroy)(struct entity_s *, struct world_s *);
    void (*render)(struct entity_s *, struct world_s *);
    void (*sync)(struct entity_s *, struct world_s *, float dt);
    void (*update)(struct entity_s *, struct world_s *, float dt);
} entity_t;

/* Creates an entity and adds it to the world. */
entity_t *entity_create(void *data, struct world_s *);

/* Initializes an entity based on their flags and init param. */
void entity_init(entity_t *, struct world_s *);

/* Destroys an entity and removes it from the world. */
void entity_destroy(entity_t *, struct world_s *);

/* Renders an entity on the screen if their render param is defined. */
void entity_render(entity_t *, struct world_s *);

/* Synchronizes an entity with the tick rate if their sync param is defined. */
void entity_sync(entity_t *, struct world_s *, float dt);

/* Updates an entity if their update param is defined. */
void entity_update(entity_t *, struct world_s *, float dt);

#endif // ifndef _ENTITY_ENTITY_H_
