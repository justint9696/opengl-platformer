#include "util/io.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void log_warn(const char *format, ...) {
    va_list args;
    char text[512];

    va_start(args, format);
    vsnprintf(text, 512, format, args);
    va_end(args);

    fprintf(stdout, "WARNING: %s", text);
}

void log_and_fail(const char *format, ...) {
    va_list args;
    char text[512];

    va_start(args, format);
    vsnprintf(text, 512, format, args);
    va_end(args);

    fprintf(stderr, "ERROR: %s", text);
    exit(EXIT_FAILURE);
}
