/**
 * @file queue.h
 * @author Justin Tonkinson
 * @date 2024/06/07
 * @brief Queue function prototypes.
 */

#ifndef _DATA_QUEUE_H_
#define _DATA_QUEUE_H_

#include <stddef.h>
#include <stdint.h>

/** @brief Queue data structure. */
typedef struct {
    /** @brief The number of items in the queue. */
    size_t len;

    /** @brief The size of each item in the queue. */
    size_t size;

    /** @brief The maximum number of items the queue can hold. */
    size_t capacity;

    /** @brief The current index of the queue. */
    uint32_t index;

    /** @brief The data section of the array. */
    void *data;
} queue_t;

/**
 * @brief Allocates a block of memory for a queue. 
 * @param self a pointer to a queue
 * @param size the size of each item contained in the array
 * @param capacity the maximum number of items the array can hold
 */
void queue_init(queue_t *, size_t size, size_t capacity);

/**
 * @brief Releases the allocated block of memory. 
 * @param self a pointer to a queue
 */
void queue_destroy(queue_t *);

/**
 * @brief Returns the next item in a queue.
 * @param self a pointer to a queue
 * @returns the next item in the queue
 */
void *queue_peak(const queue_t *);

/**
 * @brief Returns the last item in a queue.
 * @param self a pointer to a queue
 * @returns the last item in the queue
 */
void *queue_rear(const queue_t *);

/**
 * @brief Dequeues the next item in a queue and returns it.
 * @param self a pointer to a queue
 * @returns the dequeued item
 */
void *queue_pop(queue_t *);

/**
 * @brief Pushes an item at the end of a queue.
 * @param self a pointer to a queue
 */
void queue_push(queue_t *, const void *data);

/**
 * @brief Removes all items from a queue. 
 * @param self a pointer to a queue
 */
void queue_clear(queue_t *);

#endif // ifndef _DATA_QUEUE_H_
