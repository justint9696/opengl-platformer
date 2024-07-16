#include "util/log.h"

#include <stdarg.h>

void _log_message(FILE *stream, const char *title, const char *file, int line,
                  const char *format, ...) {
    va_list args;
    va_start(args, format);

    char text[1024];
    vsnprintf(text, 1024, format, args);
    fprintf(stream, "%s: %s:%d: %s", title, file, line, text);

    va_end(args);
}
