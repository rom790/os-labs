#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t pid = fork();

  if (pid < 0) {
    // Ошибка при создании процесса
    perror("Ошибка fork");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    // Код для дочернего процесса
    execl("./lb2/2/2.2/2.2-son", "2.2-son", NULL);
    // Если execl вернул управление - ошибка
    perror("Ошибка execl");
    exit(EXIT_FAILURE);
  } else {
    // Код для родительского процесса
    printf("Родительский процесс выполняется с PID: %d\n", getpid());
    printf("Создан дочерний процесс с PID: %d\n", pid);

    // Ожидаем завершения дочернего процесса
    wait(NULL);

    // Записываем состояние таблицы процессов
    system("ps -l | tee процессы_родителя.log");

    printf("Родительский процесс завершен.\n");
  }

  return 0;
}