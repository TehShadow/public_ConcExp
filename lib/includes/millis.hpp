#include <time.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

inline int64_t GetTimeMillis(void) {
    struct timespec tm;

    if (clock_gettime(CLOCK_MONOTONIC, &tm) == -1) {
        perror("clock_gettime");
        return 0;
    } else return tm.tv_sec*1000LL + tm.tv_nsec/1000000LL;
}