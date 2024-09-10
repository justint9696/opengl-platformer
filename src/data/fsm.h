/**
 * @file fsm.h
 * @author Justin Tonkinson
 * @date 2024/08/21
 * @brief Finite state machine function prototypes.
 */

#ifndef _DATA_FSM_H_
#define _DATA_FSM_H_

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
 * @note The @a table param can be NULL. If this is the case, the state table
 * will need to be built manually; @see fsm_add
 * @param self a pointer to a fsm
 * @param capacity the total number of states
 * @param table optional state table the fsm will reference for states
 */
void fsm_init(fsm_t *self, size_t capacity, int start, const state_t table[]);

/**
 * @brief Releases the memory allocated for a fsm.
 * @param self a pointer to a fsm
 */
void fsm_destroy(fsm_t *self);

/**
 * @brief Adds a state to a fsm.
 * @param self a pointer to a fsm
 * @param data the data to be copied into the state
 */
void fsm_add(fsm_t *self, const void *data);

/** @brief Internal macro to check if a state field exists and call it if so. */
#define _fsm_field(_state, _field, _T, _args, ...)\
    if ((_state)->_field) ((_T)((_state)->_field))(_args, ##__VA_ARGS__)

/**
 * @brief Updates the current state of a fsm if its on update function pointer
 * is defined.
 *
 * Usage: @code fsm_update(&fsm, void(*)(int, char), 3, 'B'); @endcode
 */
#define fsm_update(_fsm, _T, _args, ...) ({               \
    __typeof__(_fsm) __fsm = (_fsm);                      \
    state_t *_state = &__fsm->arr[__fsm->current];        \
    _fsm_field(_state, update, _T, _args, ##__VA_ARGS__); \
})

/**
 * @brief Renders the current state of a fsm if its on render function pointer
 * is defined.
 *
 * Usage: @code fsm_render(&fsm, void(*)(int, char), 3, 'B'); @endcode
 */
#define fsm_render(_fsm, _T, _args, ...) ({               \
    __typeof__(_fsm) __fsm = (_fsm);                      \
    state_t *_state = &__fsm->arr[__fsm->current];        \
    _fsm_field(_state, render, _T, _args, ##__VA_ARGS__); \
})

/**
 * @brief Transitions a fsm from one state to another.
 *
 * Usage: @code fsm_transition(&fsm, void(*)(int, char), 5, 'A'); @endcode
 */
#define fsm_transition(_fsm, _next, _T, _args, ...) ({    \
    __typeof__(_fsm) __fsm = (_fsm);                      \
    state_t *_from = &__fsm->arr[__fsm->current];         \
    _fsm_field(_from, destroy, _T, _args, ##__VA_ARGS__); \
    state_t *_to = &__fsm->arr[_next];                    \
    _fsm_field(_to, init, _T, _args, ##__VA_ARGS__);      \
    __fsm->previous = __fsm->current;                     \
    __fsm->current = (_next);                             \
})

#endif // ifndef  _DATA_FSM_H_
