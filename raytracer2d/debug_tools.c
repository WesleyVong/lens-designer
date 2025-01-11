#include "debug_tools.h"
#include "stdio.h"

StopwatchEntry * stopwatch_init(){
    StopwatchEntry * se = (StopwatchEntry *)malloc(sizeof(StopwatchEntry));
    return se;
}
void stopwatch_free(StopwatchEntry * se){
    free(se);
}
void stopwatch_print(StopwatchEntry * se, char * desc){
    clock_t elapsed = clock() - se->start_time;
    long msec = elapsed * 1000 / CLOCKS_PER_SEC;
    printf("%s: %d ms\n", desc, msec);
}
void stopwatch_reset(StopwatchEntry * se){
    se->start_time = clock();
}
void stopwatch_print_reset(StopwatchEntry * se, char * desc){
    stopwatch_print(se, desc);
    stopwatch_reset(se);
}
