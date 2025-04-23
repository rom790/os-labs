#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  struct sched_param param;
  struct timespec quantum;
  pid_t pid, ppid, child_pid;

  pid = getpid();
  ppid = getppid();
  printf("процесс: pid=%d, ppid=%d\n", pid, ppid);

  // Устанавливаем политику планирования SCHED_RR с приоритетом 99
  param.sched_priority = 99;
  if (sched_setscheduler(0, SCHED_RR, &param) == -1) {
    perror("sched_setscheduler");
    exit(EXIT_FAILURE);
  }

  // Получаем значение кванта времени для SCHED_RR
  if (sched_rr_get_interval(0, &quantum) == 0)
    printf("Полученное значение кванта: %.9f сек\n",
           quantum.tv_sec + quantum.tv_nsec / 1e9);
  else {
    perror("sched_rr_get_interval");
    exit(EXIT_FAILURE);
  }



  return 0;
}