#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  printf("Son1: Запущен (PID: %d)\nРеакция на сигнал по умолчанию\n", getpid());
  fflush(stdout);
  while (1) {
  }
  return 0;
}