#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/resource.h>

void print_scheduling_info(const char* process_name) {
    int policy = sched_getscheduler(0);
    struct sched_param param;
    sched_getparam(0, &param);
    int nice_val = getpriority(PRIO_PROCESS, 0);

    printf("[%s] PID: %d\n", process_name, getpid());
    printf("  Политика: %s\n", 
           policy == SCHED_FIFO ? "FIFO" :
           policy == SCHED_RR ? "RR" : "OTHER");
    printf("  Приоритет: %d\n", param.sched_priority);
    printf("  Nice: %d\n", nice_val);
    printf("--------------------\n");
    fflush(stdout);
}

int main() {
    printf("\n[Программа потомка] Результаты наследования через exec()\n");
    fflush(stdout);
    print_scheduling_info("Потомок exec()");
    return 0;
}