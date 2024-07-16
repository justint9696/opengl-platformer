#ifndef _UTIL_LOG_H_
#define _UTIL_LOG_H_

#include <stdio.h>
#include <stdlib.h>

void _log_message(FILE *stream, const char *title, const char *file, int line, 
                  const char *format, ...);

#ifdef _DEBUG
#define log_debug(...)\
    _log_message(stdout, "DEBUG", __FILE__, __LINE__, __VA_ARGS__);
#else
#define log_debug(...)
#endif // _DEBUG

#define log_warn(...)\
    _log_message(stdout, "WARNING", __FILE__, __LINE__, __VA_ARGS__);

#define log_and_fail(...) ({\
        _log_message(stderr, "ERROR", __FILE__, __LINE__, __VA_ARGS__);\
        exit(1);\
    })

#endif // _UTIL_LOG_H_
