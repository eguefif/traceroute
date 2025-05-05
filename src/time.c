#include "traceroute.h"

unsigned int subtract_time(struct timeval end, struct timeval start);

unsigned long get_time() {
    static struct timeval time;

    if (gettimeofday(&time, NULL) != 0) {
        fprintf(stderr, "Error: impossible to get time\n");
        exit(EXIT_FAILURE);
    }
    return time.tv_sec * 1000000 + time.tv_usec;
}

unsigned int subtract_time(struct timeval end, struct timeval start) {
    long unsigned int sec;
    long unsigned int microsec;

    sec = end.tv_sec - start.tv_sec;
    microsec = end.tv_usec - start.tv_usec;

    return (sec * 1000000) + (unsigned int)microsec;
}

int to_ms(int t) { return t / 1000; }
