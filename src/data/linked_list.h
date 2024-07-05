#ifndef _DATA_LINKED_LIST_H_
#define _DATA_LINKED_LIST_H_

#define LNODE(T) struct { T *next; }
#define LLIST(T) struct { T *head; }

#define llist_append(_list, _data) ({\
        __typeof__(_list) __list = (_list);\
        if (__list->head == NULL) {\
            __list->head = (_data);\
        } else {\
            __typeof__(__list->head) _tmp = __list->head;\
            while (_tmp->next) {\
                _tmp = _tmp->next;\
            }\
            _tmp->next = (_data);\
        }\
    })

#define llist_insert(_list, _prev, _curr, _data) ({\
        __typeof__(_list) __list = (_list);\
        if ((_prev)) {\
            (_prev)->next = (_data);\
        } else {\
            __list->head = (_data);\
        }\
        (_data)->next = (_curr);\
    })

#define llist_remove(_list, _prev, _data) ({\
        __typeof__(_list) __list = (_list);\
        if ((_prev)) {\
            (_prev)->next = (_data)->next;\
        } else {\
            __list->head = (_data)->next;\
        }\
    })

#define llist_replace(_list, _prev, _curr, _data) ({\
        __typeof__(_list) __list = (_list);\
        if ((_prev)) {\
            (_prev)->next = (_data);\
        } else {\
            __list->head = (_data);\
        }\
        (_data)->next = (_curr)->next;\
    })

#endif // _DATA_LINKED_LIST_H_
