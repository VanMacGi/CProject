#include "gtest/gtest.h"
#include <unistd.h>
#include <pthread.h>

extern "C" {
#include "perf.h"
}

char* remove_header(char *s) {
    // 'xxxxxx|string> '
    printf("%s\n", s);
    s = strchr(s, '>');
    return s + 2;
}

TEST(diff_time, first_call) {
    time_ctx ctx = {};
    char *s = diff_time("start", &ctx);
    ASSERT_STREQ("real = 0.000000s, user = 0.000000s, kernel = 0.000000s", remove_header(s));
}


TEST(diff_time, sleep_1s) {
    time_ctx ctx {};
    char *s;

    diff_time("", &ctx);
    sleep(1);
    s = diff_time("sleep", &ctx);

    ASSERT_STREQ("real = 1.000000s, user = 0.000000s, kernel = 0.000000s", remove_header(s));
}

TEST(diff_time, loop_1_bilion) {
    time_ctx ctx = {};
    char *s;
    int i = 0;

    diff_time("", &ctx);
    while (i < 1000000000) i++;
    s = diff_time("loop", &ctx);

    ASSERT_STRNE("real = 0.000000s, user = 0.000000s, kernel = 0.000000s", remove_header(s));
}

void* threadFunction(void* arg) {
    time_ctx *ctx = (time_ctx*) arg;
    int i = 0;
    char *s;
    diff_time("", ctx);
    while (i < 5) {
        sleep(1);
        i++;
    }
    s = diff_time("5s", ctx);
    pthread_exit(s);
}

TEST(diff_time, multi_threads) {
   char *s1, *s2;
   time_ctx ctx1 = {};
   time_ctx ctx2 = {};
   pthread_t thread1, thread2;
   pthread_create(&thread1, NULL, threadFunction, &ctx1);
   pthread_create(&thread2, NULL, threadFunction, &ctx2);
   pthread_join(thread1, (void**) &s1);
   pthread_join(thread2, (void**) &s2);
   ASSERT_STREQ("real = 5.000000s, user = 0.000000s, kernel = 0.000000s", remove_header(s1));
   ASSERT_STREQ("real = 5.000000s, user = 0.000000s, kernel = 0.000000s", remove_header(s2));
}

