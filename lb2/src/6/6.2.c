#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <sys/resource.h>
#include <string.h>

// Функция для вывода информации о планировании
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

// Тест наследования через fork()
void run_fork_test() {
    printf("\n[ТЕСТ 1] Наследование через fork()\n");
    fflush(stdout);
    // Настройка параметров родителя
    struct sched_param param = {.sched_priority = 50};
    if (sched_setscheduler(0, SCHED_RR, &param) == -1) {
        perror("  Ошибка установки политики RR");
    }
    nice(5);

    print_scheduling_info("Родитель (до fork)");

    pid_t pid = fork();
    if (pid == 0) {
        print_scheduling_info("Потомок fork()");
        exit(0);
    }
    wait(NULL);
}

// Тест наследования через exec()
void run_exec_test() {
    printf("\n[ТЕСТ 2] Наследование через exec()\n");
    fflush(stdout);
    // Настройка параметров родителя
    struct sched_param param = {.sched_priority = 99};
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        perror("  Ошибка установки политики FIFO");
    }
    nice(10);

    print_scheduling_info("Родитель (до exec)");

    pid_t pid = fork();
    if (pid == -1) {
        perror("Ошибка fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        // Запуск отдельной программы
        execl("./src/6/6.2-ch", "./src/6/6.2-ch", NULL);
        perror("  Ошибка при выполнении exec");
        exit(1);
    }
    wait(NULL);
}

int main() {
    printf("\n6.2 Наследование параметров планирования\n");
    fflush(stdout);
    // Запуск тестов
    run_fork_test();
    run_exec_test();

    
    return 0;
}