#include "clock.h"

#include "sokol_time.h"

static struct {
    uint64_t last_time;
    float fps;
    int frame_count;
    float elapsed_sec;
} clock;

void clock_init(void) {
    stm_setup();
}

void clock_update(void) {
    uint64_t now = stm_now();
    float dt = (float)stm_sec(stm_diff(now, clock.last_time));
    clock.last_time = now;

    clock.elapsed_sec += dt;
    clock.frame_count++;

    if (clock.elapsed_sec >= 1.0f) {
        clock.fps = (float)clock.frame_count / clock.elapsed_sec;
        clock.frame_count = 0;
        clock.elapsed_sec = 0.0f;
    }
}

float clock_fps(void) {
    return clock.fps;
}
