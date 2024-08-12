#ifndef _UTIL_ASSERT_H_
#define _UTIL_ASSERT_H_

#include "util/log.h"

#define XASSERT(_expr, _message, ...) do {\
    if (!(_expr)) {\
        log_fatal("Assertion `%s` failed: " _message, #_expr, ##__VA_ARGS__);\
    }\
} while (0)

#endif // ifndef _UTIL_ASSERT_H_
