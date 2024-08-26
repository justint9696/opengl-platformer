/**
 * @file fsm.c
 * @author Justin Tonkinson
 * @date 2024/08/21
 * @brief Finite state machine implementation functions.
 */

#include "data/fsm.h"

#include "data/array.h"

#include <assert.h>

void fsm_init(fsm_t *self, size_t capacity, int start) {
    self->current = start;
    self->previous = start;
    self->arr = array_alloc(sizeof(state_t), capacity);
}

void fsm_destroy(fsm_t *self) {
    array_free(self->arr);
}

void fsm_add(fsm_t *self, void *init, void *destroy, void *update) {
    int id = array_push(self->arr, &(state_t) {
        .init = init,
        .destroy = destroy,
        .update = update,
    });

    assert(id != -1);

    state_t *state = &self->arr[id];
    state->id = id;
}
