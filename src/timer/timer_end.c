#include <timer/timer.h>

float timer_end(void)
{
    clock_gettime(CLOCK_MONOTONIC, &timer_endtime);

    // convert ns to ms
    return timer_elapsedms = (float) (timer_endtime.tv_nsec - timer_starttime.tv_nsec) / 1000000.0;
}
