/**
 * @file entity.h
 * @author Justin Tonkinson
 * @date 2024/05/07
 * @brief Entity structures and function prototypes.
 */

#ifndef _ENTITY_ENTITY_H_
#define _ENTITY_ENTITY_H_

#include "game/types.h"

#include <stdint.h>

struct world_s;

/** @brief Flags that determine an entity's behavior. */
typedef enum entity_flag_e {
    /** @brief The entity will respond to player input. */
    EF_PLAYER_CONTROLLED = 1 << 0,

    /** @brief The entity is controlled by an AI. */
    EF_AI_CONTROLLED = 1 << 1,

    /** @brief The entity is expected to move. */
    EF_KINEMATIC = 1 << 2,

    /** @brief The entity will respond to gravity. */
    EF_GRAVITY = 1 << 3,

    /** @brief The camera will follow the entity's movement. */
    EF_CAMERA_FOLLOW = 1 << 4,

    /** @brief The entity has no flags. */
    EF_NONE = 0,
} entity_flag_t;

/** @brief Every type of entity. */
typedef enum entity_type_e {
    ET_PLAYER,
    ET_ENEMY,
    ET_PLATFORM,
    ET_MAX,
} entity_type_t;

/** @brief Entity data structure. */
typedef struct entity_s {
    /** @brief Entity identification number. */
    int32_t id;

    /** @brief The flags the entity has. */
    int32_t flags;

    /** @brief Button bit mask. */
    int32_t buttons;

    /** @brief The type of entity. */
    entity_type_t type;

    /** @brief The physical body of the entity. */
    body_t body;

    /** @brief Optional initialize function pointer. */
    void (*init)(struct entity_s *, struct world_s *);

    /** @brief Optional destroy function pointer. */
    void (*destroy)(struct entity_s *, struct world_s *);

    /** @brief Optional render function pointer. */
    void (*render)(struct entity_s *, struct world_s *);

    /** @brief Optional synchronize function pointer. */
    void (*sync)(struct entity_s *, struct world_s *, float dt);

    /** @brief Option update function pointer. */
    void (*update)(struct entity_s *, struct world_s *, float dt);
} entity_t;

/**
 * @brief Creates an entity and adds it to the world.
 * @param data the data to be copied into the entity
 * @param world a pointer to the game world
 * @returns a pointer to the entity
 */
entity_t *entity_create(const void *data, struct world_s *);

/**
 * @brief Initializes an entity based on their flags and init param.
 * @param self a pointer to an entity
 * @param world a pointer to the game world
 */
void entity_init(entity_t *, struct world_s *);

/**
 * @brief Destroys an entity and removes it from the world.
 * @param self a pointer to an entity
 * @param world a pointer to the game world
 */
void entity_destroy(entity_t *, struct world_s *);

/**
 * @brief Renders an entity on the screen if their render param is defined.
 * @param self a pointer to an entity
 * @param world a pointer to the game world
 */
void entity_render(entity_t *, struct world_s *);

/**
 * @brief Synchronizes an entity with the tick rate if their sync param is
 * defined.
 * @param self a pointer to an entity
 * @param world a pointer to the game world
 * @param dt delta time
 */
void entity_sync(entity_t *, struct world_s *, float dt);

/** Updates an entity if their update param is defined.
 * @param self a pointer to an entity
 * @param world a pointer to the game world
 * @param dt delta time
 */
void entity_update(entity_t *, struct world_s *, float dt);

#endif // ifndef _ENTITY_ENTITY_H_
