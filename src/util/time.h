/**
 * @file time.h
 * @author Justin Tonkinson
 * @date 2024/05/06
 * @brief Time management data structure and function prototypes.
 */

#ifndef _UTIL_TIME_H_
#define _UTIL_TIME_H_

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define NS_PER_SECOND 1000000000
#define MS_PER_SECOND 1000

#define NS_PER_MS     (NS_PER_SECOND / MS_PER_SECOND)

#define TICK_TARGET   50
#define FRAME_TARGET  60

#define TICK_DELAY    (NS_PER_SECOND / TICK_TARGET)
#define FRAME_DELAY   (NS_PER_SECOND / FRAME_TARGET)

#define DELTA_FIXED   ((double)TICK_DELAY / (double)NS_PER_SECOND)

/** @brief Returns the current UTC time in nanoseconds. */
#define NOW() ({\
        struct timespec ts;\
        timespec_get(&ts, TIME_UTC);\
        ((ts.tv_sec * NS_PER_SECOND) + ts.tv_nsec);\
    })

/** @brief Time management data structure. */
struct time_s {
    time_t delta_ns;
    time_t last_second, last_tick, last_frame;
    float delta, delta_fixed;
    uint32_t frames, ticks;
    uint32_t fps, tps;
};

/**
 * @brief Initializes the time structure.
 * @param self a pointer to a time manager
 */
void time_init(struct time_s *);

/**
 * @brief Updates the time structure.
 * @param self a pointer to a time manager
 */
void time_update(struct time_s *);

/**
 * @brief Determines if the game should synchronize and updates the time manager
 * accordingly.
 * @param self a pointer to a time manager
 * @returns true if the the time since the last tick is greater than the tick
 * delay or false otherwise
 */
bool time_sync(struct time_s *);

/**
 * @brief Returns the time passed since the provided tick in nanoseconds.
 * @param tick a point in time
 */
time_t time_since(time_t);

/**
 * @brief Returns the time passed since the provided tick in milliseconds.
 * @param tick a point in time
 */
time_t time_since_ms(time_t);

#endif // ifndef _UTIL_TIME_H_
