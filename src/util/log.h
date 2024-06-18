#ifndef _UTIL_LOG_H_
#define _UTIL_LOG_H_

#include <stdio.h>
#include <stdlib.h>

#ifdef _DEBUG
#define log_debug(...) fprintf(stdout, "DEBUG: " __VA_ARGS__);
#else
#define log_debug(...)
#endif // _DEBUG

#define log_warn(...) fprintf(stdout, "WARNING: " __VA_ARGS__);

#define log_and_fail(...) ({ fprintf(stderr, "ERROR: " __VA_ARGS__); exit(1); })

#endif // _UTIL_LOG_H_
