#ifndef _DATA_FSM_H_
#define _DATA_FSM_H_

#include <stddef.h>

typedef struct state_s {
    // state identifier
    int id;

    // optional on enter function pointer
    void *init;

    // optional on exit function pointer
    void *destroy;

    // optional on update function pointer
    void *update;
} state_t;

typedef struct {
    // current state
    int current;

    // previous state
    int previous;

    // dynamic array of all possible states
    state_t *arr;
} fsm_t;

/* Allocates memory for a fsm. */
void fsm_init(fsm_t *, size_t capacity, int start);

/* Releases the memory allocated for a fsm. */
void fsm_destroy(fsm_t *);

/* Adds a state to a fsm. */
void fsm_add(fsm_t *, void *init, void *destroy, void *update);

/* Updates the current state of an fsm if it exists. */
#define fsm_update(_fsm, _T, _args, ...) ({           \
    __typeof__(_fsm) __fsm = (_fsm);                  \
    state_t *_state = &__fsm->arr[__fsm->current];    \
    if (_state->update) {                             \
        ((_T)(_state->update))(_args, ##__VA_ARGS__); \
    }                                                 \
})

/* Transitions a fsm from one state to another. */
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
