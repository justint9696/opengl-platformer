#ifndef __STATUS_H__
#define __STATUS_H__

#include <stdio.h>

#define S_SUCCESS "\033[1;32mPassed\033[0m"
#define S_FAILURE "\033[1;31mFailed\033[0m"

#define log_status(_name, _ret)                       \
    fprintf(stdout, "%s...\t%s\n", _name,             \
            (_ret == -1) ? (S_FAILURE) : (S_SUCCESS))

#endif
