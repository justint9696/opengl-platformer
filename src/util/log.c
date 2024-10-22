/**
 * @file log.c
 * @author Justin Tonkinson
 * @data 2024/10/18
 * @brief This file contains the function implementations for custom logging.
 */

#include "util/log.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

static FILE *fp;

void _log_message(int level, const char *file, const char *function, int line,
                  const char *format, ...) {
    if (!fp) {
        fp = fopen("game.log", "w+");
    }

    va_list args;
    char text[1024];

    va_start(args, format);
    vsnprintf(text, 1024, format, args);
    va_end(args);

    time_t now;
    char buf[128];

    time(&now);
    strftime(buf, sizeof(buf), "%H:%M:%S", localtime(&now));

    fprintf(fp, "[%s][%s:%d: %s]: %s\n",
            buf, file, line, function, text);

#ifdef _DEBUG
    fprintf(stdout, "[%s:%d: %s]: %s\n", file, line, function, text);
#endif // ifdef _DEBUG

    switch (level) {
        case LOG_FATAL:
        case LOG_ERROR:
            exit(1);
        default:
            break;
    }
}
