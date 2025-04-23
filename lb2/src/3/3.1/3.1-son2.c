#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  printf("Son2: Запущен (PID: %d)\nИгнорирование сигнала от родительского процесса\n", getpid());
  fflush(stdout);
  signal(SIGINT, SIG_IGN); // Игнорируем SIGINT
  // Имитация работы
  for (int counter, i = 0; i < 1000000000; i++){
    counter++;
  }
  printf("Son2: Завершился.\n");
  fflush(stdout);
  exit(0);
}