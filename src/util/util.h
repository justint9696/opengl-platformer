#ifndef _UTIL_UTIL_H_
#define _UTIL_UTIL_H_

#define min(_a, _b) ({\
        __typeof__(_a) a = (_a);\
        __typeof__(_b) b = (_b);\
        a > b ? b : a;\
    })

#define max(_a, _b) ({\
        __typeof__(_a) a = (_a);\
        __typeof__(_b) b = (_b);\
        a < b ? b : a;\
    })

#define clamp(_n, _mn, _mx) ({\
        __typeof__(_n) n = (_n);\
        __typeof__(_mn) mn = (_mn);\
        __typeof__(_mx) mx = (_mx);\
        n < mn ? mn : n > mx ? mx : n;\
    })

#endif
