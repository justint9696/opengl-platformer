/**
 * @file assert.h
 * @author Justin Tonkinson
 * @date 2024/08/12
 * @brief Custom assertion macro.
 */

#ifndef _UTIL_ASSERT_H_
#define _UTIL_ASSERT_H_

#include "util/log.h"

/**
 * @brief Determines if an expression is true and logs an error message if it
 * is not.
 */
#define XASSERT(_expr, _message, ...) do {      \
        if (!(_expr)) {                         \
            log_fatal(_message, ##__VA_ARGS__); \
        }                                       \
    } while (0)

#endif // ifndef _UTIL_ASSERT_H_
