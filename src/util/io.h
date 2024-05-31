#ifndef _UTIL_IO_H_
#define _UTIL_IO_H_

#include <stdio.h>

#define ASSERT(_e, ...) ({\
        if (!(_e)) { fprintf(stderr, "[ERROR] " __VA_ARGS__); exit(1); }\
    })

#define WARN(_e, ...) ({\
        if (!(_e)) { fprintf(stderr, "[WARNING] " __VA_ARGS__); }\
    })

#ifdef DEBUG
#define LOG(...) fprintf(stdout, "[DEBUG] " __VA_ARGS__);
#else 
#define LOG(...) 
#endif

#endif
