#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handle_signal(int sig) {
  printf("Son3: Сигнал получен, процесс продолжает работу sig(%d)\n", sig);
  fflush(stdout);
}

int main() {
  printf("Son3: Запущен (PID: %d)\n", getpid());
  fflush(stdout);
  signal(SIGINT, handle_signal); // Устанавливаем обработчик для SIGINT
  for (int counter, i = 0; i < 1000000000; i++){
    counter++;
  }
  printf("Son3: Завершился.\n");
  fflush(stdout);
  exit(0);
}