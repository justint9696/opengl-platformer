#ifndef _UTIL_TIME_H_
#define _UTIL_TIME_H_

#include <inttypes.h>
#include <stdbool.h>
#include <time.h>

#define NS_PER_SECOND 1000000000
#define MS_PER_SECOND 1000

#define NS_PER_MS     (NS_PER_SECOND / MS_PER_SECOND)

#define TICK_TARGET   50
#define FRAME_TARGET  60

#define TICK_DELAY    (NS_PER_SECOND / TICK_TARGET)
#define FRAME_DELAY   (NS_PER_SECOND / FRAME_TARGET)

#define DELTA_FIXED   ((double)TICK_DELAY / (double)NS_PER_SECOND)

#define NS(_ms)       ((_ms) * NS_PER_MS)
#define MS(_ns)       ((_ns) / NS_PER_MS)

#define NOW() ({\
        struct timespec ts;\
        timespec_get(&ts, TIME_UTC);\
        ((ts.tv_sec * NS_PER_SECOND) + ts.tv_nsec);\
    })

typedef struct {
    time_t delta_ns;
    time_t last_second, last_tick, last_frame;
    float delta, delta_fixed;
    uint32_t frames, ticks;
    uint32_t fps, tps;
} time_s;

void time_init(time_s *);
void time_update(time_s *);

bool time_tick(time_s *);

#endif
