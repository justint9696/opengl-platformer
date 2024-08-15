#ifndef _UTIL_ASSERT_H_
#define _UTIL_ASSERT_H_

#include "util/log.h"

#define XASSERT(_expr, _message, ...) ({\
        if (!(_expr)) {\
            log_fatal(_message, ##__VA_ARGS__);\
        }\
    })

#endif // ifndef _UTIL_ASSERT_H_
