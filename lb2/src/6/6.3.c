#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>




void signal_handler(int sig) {
    printf("[Сигнал] Процесс (PID %d) получил сигнал %d\n", getpid(), sig);
    fflush(stdout);
}

void fork_signal_test(void) {
    printf("\n[ТЕСТ 1] Наследование через fork()\n");
    fflush(stdout);

    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("Ошибка sigaction");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Ошибка fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        for (int counter, i =0; i < 10000000; i++) counter++;
        printf("[Потомок] PID %d: Отправляю SIGUSR1 себе\n", getpid());
        fflush(stdout);
        raise(SIGUSR1);
        exit(EXIT_SUCCESS);
    } else {
        for (int counter, i =0; i < 10000000; i++) counter++;
        printf("[Родитель] PID %d: Отправляю SIGUSR1 потомку (PID %d)\n", getpid(), pid);
        fflush(stdout);
        raise(SIGUSR1);
        wait(NULL);
    }

    printf("Тест наследования сигналов через fork() завершён.\n\n");
    fflush(stdout);
}

void exec_signal_test() {
    printf("[ТЕСТ 3] Наследование через exec()\n");
    fflush(stdout);

    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("Ошибка sigaction");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGUSR2, &sa, NULL) == -1) {
        perror("Ошибка sigaction");
        exit(EXIT_FAILURE);
    }
    printf("Родитель отправляет себе сигнал %d\n", SIGUSR1);
    fflush(stdout);
    raise(SIGUSR1);

    printf("Родитель отправляет себе сигнал %d\n", SIGUSR1);
    fflush(stdout);
    raise(SIGUSR2);


    pid_t pid = fork();
    if (pid < 0) {
        perror("Ошибка fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        execl("./src/6/6.3-ch", "./src/6/6.3-ch", "exec_child_signal", NULL);
        perror("Ошибка execl");
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
    }

    printf("Тест наследования сигналов через exec() завершён.\n\n");
    fflush(stdout);
}
int main() {
    printf("\n6.3 Наследование диспозиции сигналов\n");
    fflush(stdout);
    fork_signal_test();
    exec_signal_test();
    
    printf("1. При fork() обработчики сохраняются\n");
    printf("2. При exec() все обработчики сбрасываются в SIG_DFL и их нужно восстанавливать\n");
    fflush(stdout);
    return 0;
}