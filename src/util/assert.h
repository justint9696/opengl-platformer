#ifndef _UTIL_ASSERT_H_
#define _UTIL_ASSERT_H_

#include "util/log.h"

/* Ensures the given expression is true. If not, the error is logged and the
 * program terminates. */
#define XASSERT(_expr, _message, ...) do {\
        if (!(_expr)) {\
            log_fatal(_message, ##__VA_ARGS__);\
        }\
    } while (0)

#endif // ifndef _UTIL_ASSERT_H_
