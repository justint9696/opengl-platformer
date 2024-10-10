/**
 * @file array.h
 * @author Justin Tonkinson
 * @date 2024/05/07
 * @brief Dynamic array function prototypes.
 */

#ifndef _DATA_ARRAY_H_
#define _DATA_ARRAY_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/** @brief Internal header that manages arrays. */
typedef struct array_s {
    /** @brief Dynamically resize the array. */
    bool dynamic;

    /** @brief The number of items in the array. */
    size_t len;

    /** @brief The size of each item in the array. */
    size_t size;

    /** @brief The maximum number of items the array can hold. */
    size_t capacity;

    /** @brief The data section of the array. */
    void *data;
} array_t;

/** @brief Returns the header of an array's data section. */
#define array_header(_array)\
    ((array_t *)(((void *)(_array)) - offsetof(array_t, data)))

/**
 * @brief Initializes an array at the given memory address.
 * @param ptr a pointer to a block of memory
 * @param size the size of each item contained in the array
 * @param capacity the maximum number of items the array can hold
 * @returns a pointer to the data section of the array
 */
void *array_init(void *ptr, size_t size, size_t capacity);

/**
 * @brief Allocates memory for an array.
 * @param size the size of each item contained in the array
 * @param capacity the maximum number of items the array can hold
 * @returns a pointer to the data section of the array
 */
void *array_alloc(size_t size, size_t capacity);

/**
 * @brief Releases the memory allocated for an array.
 * @param ptr a pointer to the data section of an array
 */
void array_free(void *ptr);

/**
 * @brief Removes all items from an array.
 * @param ptr a pointer to the data section of an array
 */
void array_clear(void *ptr);

/**
 * @brief Reallocates memory for an array with updated capacity.
 *
 * The items in the array before reallocation will be kept, only the capacity is
 * affected.
 *
 * @param ptr a pointer to the data section of an array
 * @param capacity the maximum number of items the array can hold
 * @returns a pointer to the data section of the reallocated array
 */
void *array_resize(void *ptr, size_t capacity);

/**
 * @brief Pushes an item to the end of an array.
 * @param ptr a pointer to the data section of an array
 * @param item a pointer to the item being inserted into the array
 * @returns the index of insertion on success or -1 on failure
 */
int _array_push_intern(void **dataptr, const void *item);

/**
 * @brief Macro to automatically convert the (void *) to a (void **) to
 * dynamically resize the array and surpress conversion warnings.
 */
#define array_push(_ptr, _data) _array_push_intern((void **)&(_ptr), (_data))

/**
 * @brief Removes an item from an array.
 * @param ptr a pointer to the data section of an allocated array
 * @param data the item to be removed
 * @returns the size of the array on success or -1 on failure
 */
int array_remove(void *ptr, const void *item);

/**
 * @brief Returns a pointer to the item at an index of an array.
 * @param ptr a pointer to the data section of an array
 * @param index the index of an item within an array
 * @returns the item at the given index or NULL if the index is invalid
 */
void *array_at(void *ptr, uint32_t index);

/**
 * @brief Returns the number of items in an array.
 * @param ptr a pointer to the data section of an array
 * @returns the length of the array
 */
size_t array_len(void *ptr);

/**
 * @brief Allocates memory for an array and copies the items from the indices
 * start (inclusive) to end (inclusive).
 * @param ptr a pointer to the data section of an array
 * @param start the starting index
 * @param end the ending index
 * @returns a pointer to the data section of allocated array
 */
void *array_slice(void *ptr, uint32_t start, uint32_t end);

/**
 * @brief Copies the contents from the source array to the destination array.
 * @param dst a pointer to the data section of the destination array
 * @param src a pointer to the data section of the source array
 */
void array_copy(void *dst, void *src);

/**
 * @brief Inserts an item at a specific index of an array.
 * @note The @a index must be between 0 and the length of the array. This
 * function is not meant to reserve an index, but instead to modify the indices
 * of an already existing array.
 * @param ptr a pointer to the data section of an array
 * @param index the index of an array
 * @param item a pointer to the item being inserted into the array
 */
void array_insert(void *ptr, uint32_t index, const void *item);

#endif // ifndef _DATA_ARRAY_H_
