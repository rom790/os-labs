#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>


void signal_handler(int sig) {
    printf("[Сигнал] Процесс (PID %d) получил сигнал %d\n", getpid(), sig);
    fflush(stdout);
}

void print_signal_disposition(const char* process_name) {
    printf("\n[%s] PID: %d\n", process_name, getpid());
    fflush(stdout);
    for (int sig = SIGUSR1; sig <= SIGUSR2; sig++) {
        struct sigaction oldact;
        sigaction(sig, NULL, &oldact);
        
        printf("  SIG%d (%s): ", sig, strsignal(sig));
        if (oldact.sa_handler == SIG_DFL) {
            printf("по умолчанию\n");
        } else if (oldact.sa_handler == SIG_IGN) {
            printf("игнорируется\n");
        } else {
            printf("пользовательская диспозиция\n");
        }
    }
    printf("--------------------\n");
    fflush(stdout);
    // Проверка реакции на сигналы
    printf("Посылаем SIGUSR1 программе...\n");
    fflush(stdout);
    kill(getpid(), SIGUSR1);
    printf("Посылаем SIGUSR2 программе...\n");
    fflush(stdout);
    kill(getpid(), SIGUSR2);
}

int main() {
    printf("\n[Программа потомка] Наследование через exec()\n");
    printf("Восстановим обработчик для сигнала %d", SIGUSR1);
    fflush(stdout);
    
    signal(SIGUSR1, signal_handler);
    print_signal_disposition("Потомок exec()");
    return 0;
}