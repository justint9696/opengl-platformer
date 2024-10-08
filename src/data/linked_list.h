/**
 * @file linked_list.h
 * @author Justin Tonkinson
 * @date 2024/06/27
 * @brief Macros for singly linked list objects.
 */

#ifndef _DATA_LINKED_LIST_H_
#define _DATA_LINKED_LIST_H_

/** @brief Creates a singly linked list node structure. */
#define LNODE(T) struct { T *next; }

/** @brief Creates a singly linked list structure. */
#define LLIST(T) struct { T *head; }

/** @brief Adds an item to the end of a linked linked. */
#define llist_append(_list, _data) ({                     \
        __typeof__(_list) __list = (_list);               \
        if (__list->head == NULL) {                       \
            __list->head = (_data);                       \
        } else {                                          \
            __typeof__(__list->head) _tmp = __list->head; \
            while (_tmp->next) {                          \
                _tmp = _tmp->next;                        \
            }                                             \
            _tmp->next = (_data);                         \
        }                                                 \
    })

/** @brief Inserts an item at the current node of a linked list. */
#define llist_insert(_list, _prev, _curr, _data) ({ \
        __typeof__(_list) __list = (_list);         \
        if ((_prev)) {                              \
            (_prev)->next = (_data);                \
        } else {                                    \
            __list->head = (_data);                 \
        }                                           \
        (_data)->next = (_curr);                    \
    })

/** @brief Removes an item from a linked list. */
#define llist_remove(_list, _prev, _data) ({ \
        __typeof__(_list) __list = (_list);  \
        if ((_prev)) {                       \
            (_prev)->next = (_data)->next;   \
        } else {                             \
            __list->head = (_data)->next;    \
        }                                    \
    })

/** @brief Replaces the current node of a linked list. */
#define llist_replace(_list, _prev, _curr, _data) ({ \
        __typeof__(_list) __list = (_list);          \
        if ((_prev)) {                               \
            (_prev)->next = (_data);                 \
        } else {                                     \
            __list->head = (_data);                  \
        }                                            \
        (_data)->next = (_curr)->next;               \
    })

#endif // ifndef _DATA_LINKED_LIST_H_
