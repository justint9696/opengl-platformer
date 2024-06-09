#ifndef _UTIL_IO_H_
#define _UTIL_IO_H_

#include <stdio.h>

#ifdef DEBUG
#define LOG(...) (fprintf(stdout, "LOG: " __VA_ARGS__))
#else 
#define LOG(...) 
#endif

void log_warn(const char *format, ...);
void log_and_fail(const char *format, ...);

#endif
