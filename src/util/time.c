#include "util/time.h"

#include <string.h>

void time_init(struct time_s *self) {
    memset(self, 0, sizeof(struct time_s));
    const time_t now = NOW();
    self->last_tick = now;
    self->last_second = now;
    self->delta_fixed = DELTA_FIXED;
}

void time_update(struct time_s *self) {
    const time_t now = NOW();
    self->delta_ns = now - self->last_frame;
    self->delta
        = ((self->delta_ns) ? (1.f * self->delta_ns / NS_PER_SECOND) : 1.f);

    self->last_frame = now;

    self->frames++;

    if (now - self->last_second > NS_PER_SECOND) {
        self->last_second = now;

        self->fps = self->frames;
        self->tps = self->ticks;

        self->frames = 0;
        self->ticks = 0;
    }
}

bool time_sync(struct time_s *self) {
    const time_t now = NOW();
    if (now - self->last_tick > TICK_DELAY) {
        self->ticks++;
        self->last_tick += TICK_DELAY;
        return true;
    }
    return false;
}

time_t time_since(time_t tick) {
    const time_t now = NOW();
    return (now - tick);
}

time_t time_since_ms(time_t tick) {
    const time_t now = NOW();
    return (now - tick) / NS_PER_MS;
}
