#include <pthread.h>
#include <sys/times.h>

typedef struct _time_txt {
    pthread_t th;
    struct tms cpu;
    clock_t time;
    char log[200];
} time_ctx;

char* diff_time(const char* stage, time_ctx *last);

