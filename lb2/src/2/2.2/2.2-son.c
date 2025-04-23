#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  printf("Дочерний процесс выполняется с PID: %d\n", getpid());

  // Записываем состояние таблицы процессов
  for (int counter= 0, i= 0;i  <900000000; i++) {
    counter++;
  }
  system("ps -l | tee процессы_дочернего.log");

  printf("Дочерний процесс завершен.\n");

  return 0;
}