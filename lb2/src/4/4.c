#define _GNU_SOURCE
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>

#define THREAD_STACK_CAPACITY (1024 * 1024) // 1 МБ стека для каждого потока
#define THREAD_AMOUNT 3

volatile sig_atomic_t termination_requested = 0; // Флаг для остановки потоков

// Обработчик сигналов
void handle_signal(int signal_num) {
  termination_requested = 1;
  printf("Получен сигнал %d, остановка потоков...\n", signal_num);
  fflush(stdout);
}

// Функция для потоков, созданных через clone()
int cloned_thread_worker(void *thread_data) {
  int worker_id = *(int *)thread_data;
  while (!termination_requested) {
    printf("Созданый через clone поток %d (TID: %ld)\n", worker_id, (long)syscall(SYS_gettid));
    fflush(stdout);
    for (int counter, i = 0; i < 500000000; i++) {
      counter++;
    }
  }
  printf("Клонированный поток %d завершился\n", worker_id);
  fflush(stdout);
  return 0;
}
  
// Функция для потоков, созданных через pthread
void *pthread_worker(void *thread_data) {
  int worker_id = *(int *)thread_data;
  while (!termination_requested) {
    printf("Поток, созданный Pthread %d (TID: %ld)\n", worker_id, (long)syscall(SYS_gettid));
    fflush(stdout);
    for (int counter, i = 0; i < 500000000; i++) {
      counter++;
    }
  }
  printf("Pthread поток %d завершился\n", worker_id);
  fflush(stdout);
  return NULL;
}

int main(void) {
  // Настройка обработчика сигналов
  struct sigaction signal_config;
  signal_config.sa_handler = handle_signal;
  sigemptyset(&signal_config.sa_mask);
  signal_config.sa_flags = 0;

  if (sigaction(SIGINT, &signal_config, NULL) == -1 ||
      sigaction(SIGTERM, &signal_config, NULL) == -1) {
    perror("Ошибка настройки обработчика сигналов");
    exit(EXIT_FAILURE);
  }

  pthread_t pthread_workers[THREAD_AMOUNT];
  int thread_ids[THREAD_AMOUNT * 2]; // Идентификаторы для всех потоков
  char *thread_stacks[THREAD_AMOUNT];
  pid_t cloned_workers[THREAD_AMOUNT];

  // Инициализация идентификаторов потоков
  for (int i = 0; i < THREAD_AMOUNT * 2; i++) {
    thread_ids[i] = i + 1;
  }

  // Создание потоков через pthread_create()
  for (int i = 0; i < THREAD_AMOUNT; i++) {
    if (pthread_create(&pthread_workers[i], NULL, pthread_worker, &thread_ids[i]) != 0) {
      fprintf(stderr, "Ошибка создания pthread потока: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }
  }

  // Создание потоков через clone()
  for (int i = 0; i < THREAD_AMOUNT; i++) {
    thread_stacks[i] = malloc(THREAD_STACK_CAPACITY);
    if (!thread_stacks[i]) {
      perror("Ошибка выделения памяти для стека");
      exit(EXIT_FAILURE);
    }

    cloned_workers[i] = clone(cloned_thread_worker, thread_stacks[i] + THREAD_STACK_CAPACITY,
                          CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND |
                              CLONE_THREAD | CLONE_SYSVSEM,
                          &thread_ids[i + THREAD_AMOUNT]);

    if (cloned_workers[i] == -1) {
      fprintf(stderr, "Ошибка создания clone потока: %s\n", strerror(errno));
      free(thread_stacks[i]);
      exit(EXIT_FAILURE);
    }
  }

  // Ожидание сигнала для завершения
  while (!termination_requested) {
    for (int counter, i = 0; i < 100000000; i++) {
      counter++;
    }
  }

  // Ожидание завершения pthread потоков
  for (int i = 0; i < THREAD_AMOUNT; i++) {
    if (pthread_join(pthread_workers[i], NULL) != 0) {
      fprintf(stderr, "Ошибка ожидания pthread потока: %s\n", strerror(errno));
    }
  }

  // Ожидание завершения clone потоков
  for (int i = 0; i < THREAD_AMOUNT; i++) {
    if (waitpid(cloned_workers[i], NULL, 0) == -1) {
      fprintf(stderr, "Ошибка ожидания clone потока: %s\n", strerror(errno));
    }
    free(thread_stacks[i]);
  }

  printf("Все потоки завершили выполнение\n");
  fflush(stdout);
  return EXIT_SUCCESS;
}