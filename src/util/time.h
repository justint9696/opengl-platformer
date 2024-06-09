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

#define NOW() ({\
        struct timespec ts;\
        timespec_get(&ts, TIME_UTC);\
        ((ts.tv_sec * NS_PER_SECOND) + ts.tv_nsec);\
    })

struct time_s {
    time_t delta_ns;
    time_t last_second, last_tick, last_frame;
    float delta, delta_fixed;
    uint32_t frames, ticks;
    uint32_t fps, tps;
};

void time_init(struct time_s *);
void time_update(struct time_s *);

bool time_sync(struct time_s *);

time_t time_since(time_t);
time_t time_since_ms(time_t);

#endif
