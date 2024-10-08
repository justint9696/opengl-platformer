/**
 * @file log.h
 * @author Justin Tonkinson
 * @date 2024/06/18
 * @brief Custom logging macros.
 */

#ifndef _UTIL_LOG_H_
#define _UTIL_LOG_H_

#include <stdio.h>
#include <stdlib.h>

#ifndef _DEBUG
#define _DEBUG 1
#endif // ifndef _DEBUG

#ifdef _DEBUG
#define log_debug(_message, ...)                                \
    fprintf(stdout, "[DEBUG][%s:%u]: "                          \
            _message, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define log_debug(_message, ...)
#endif // ifdef _DEBUG

#define log_warn(_message, ...)                                 \
    fprintf(stdout, "[WARNING][%s:%u]: "                        \
            _message, __FILE__, __LINE__, ##__VA_ARGS__)

#define log_fatal(_message, ...) ({\
        fprintf(stderr, "[ERROR][%s:%u]: "                      \
                _message, __FILE__, __LINE__, ##__VA_ARGS__);   \
        exit(1);                                                \
    })

#endif // ifndef _UTIL_LOG_H_
