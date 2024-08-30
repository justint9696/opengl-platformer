/**
 * @file fsm.h
 * @author Justin Tonkinson
 * @date 2024/08/21
 * @brief Finite state machine function prototypes.
 */

#ifndef _DATA_FSM_H_
#define _DATA_FSM_H_

#include <stddef.h>

#include <stddef.h>

/** @brief Represents a state of a finite state machine. */
typedef struct state_s {
    /** @brief State identifier. */
    int id;

    /** @brief Optional on enter function pointer. */
    void *init;

    /** @brief Optional on exit function pointer. */
    void *destroy;

    /** @brief Optional on update function pointer. */
    void *update;

    /** @brief Optional on render function pointer. */
    void *render;
} state_t;

/** @brief Finite state machine data structure. */
typedef struct fsm_s {
    /** @brief Previous state of the finite state machine. */
    int previous;

    /** @brief Current state of the finite state machine. */
    int current;

    /** @brief Dynamic array of all possible states. */
    state_t *arr;
} fsm_t;

/**
 * @brief Allocates memory for a fsm. 
 * @param self a pointer to a fsm
 * @param capacity the total number of states
 */
void fsm_init(fsm_t *self, size_t capacity, int start);

/**
 * @brief Releases the memory allocated for a fsm.
 * @param self a pointer to a fsm
 */
void fsm_destroy(fsm_t *self);

/**
 * @brief Adds a state to a fsm.
 * @param self a pointer to a fsm
 * @param data the data that will be copied into the state
 */
void fsm_add(fsm_t *self, const void *data);

#define _fsm_field(_name)

/**
 * @brief Updates the current state of a fsm if its on update function pointer
 * is defined.
 *
 * Usage: @code fsm_update(&fsm, void(*)(int, char), 3, 'B'); @endcode
 */
#define fsm_update(_fsm, _T, _args, ...) ({           \
    __typeof__(_fsm) __fsm = (_fsm);                  \
    state_t *_state = &__fsm->arr[__fsm->current];    \
    if (_state->update) {                             \
        ((_T)(_state->update))(_args, ##__VA_ARGS__); \
    }                                                 \
})

/**
 * @brief Renders the current state of a fsm if its on render function pointer
 * is defined.
 *
 * Usage: @code fsm_render(&fsm, void(*)(int, char), 3, 'B'); @endcode
 */
#define fsm_render(_fsm, _T, _args, ...) ({           \
    __typeof__(_fsm) __fsm = (_fsm);                  \
    state_t *_state = &__fsm->arr[__fsm->current];    \
    if (_state->render) {                             \
        ((_T)(_state->render))(_args, ##__VA_ARGS__); \
    }                                                 \
})

/**
 * @brief Transitions a fsm from one state to another. 
 *
 * Usage: @code fsm_transition(&fsm, void(*)(int, char), 5, 'A'); @endcode
 */
#define fsm_transition(_fsm, _next, _T, _args, ...) ({ \
    __typeof__(_fsm) __fsm = (_fsm);                   \
    state_t *_from = &__fsm->arr[__fsm->current];      \
    state_t *_to = &__fsm->arr[_next];                 \
    if (_from->destroy) {                              \
        ((_T)(_from->destroy))(_args, ##__VA_ARGS__);  \
    }                                                  \
    if (_to->init) {                                   \
        ((_T)(_to->init))(_args, ##__VA_ARGS__);       \
    }                                                  \
    __fsm->previous = __fsm->current;                  \
    __fsm->current = (_next);                          \
})

#endif // ifndef  _DATA_FSM_H_
