#include <stdbool.h>
#include <stdio.h>

#include "perf.h"

char* diff_time(const char* stage, time_ctx *last ) {
    struct tms cr_cpu;
    clock_t cr_time;

    if (last->th == 0)
        last->th = pthread_self();

    cr_time = times(&cr_cpu);

    sprintf(last->log, "%lu|%s> real = %fs, user = %fs, kernel = %fs",
        last->th,
        stage,
        1.0 * (last->time == 0 ? 0 : (cr_time - last->time)) / 100,
        1.0 * (cr_cpu.tms_utime - last->cpu.tms_utime) / 100,
        1.0 * (cr_cpu.tms_stime - last->cpu.tms_stime) / 100
    );

    last->time = times(&(last->cpu));

    return last->log;
}
