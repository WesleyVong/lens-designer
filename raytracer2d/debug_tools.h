#ifndef DEBUG_TOOLS_H
#define DEBUG_TOOLS_H

#include "time.h"

typedef struct {
    clock_t start_time;
} StopwatchEntry;

StopwatchEntry * stopwatch_init();
void stopwatch_free(StopwatchEntry * se);
void stopwatch_print(StopwatchEntry * se, char * desc);
void stopwatch_print_reset(StopwatchEntry * se, char * desc);
void stopwatch_reset(StopwatchEntry * se);

#endif