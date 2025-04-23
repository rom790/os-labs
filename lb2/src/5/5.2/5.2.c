#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>

// Функция для получения и вывода текущего приоритета процесса
void print_priority(const char *process_name, pid_t pid) {
  int prio =
      getpriority(PRIO_PROCESS, pid); // Получаем приоритет для указанного PID
  if (prio == -1 && errno != 0) {
    perror("Ошибка при получении приоритета");
  } else {
    printf("%-40s (PID: %d): Текущий приоритет (значение nice): %d\n",
           process_name, pid, prio);
  }
}

int main() {
  // Пример системного процесса
  pid_t system_process_pid = 15; 
  printf("Сравнение и изменение приоритетов системного и пользовательского "
         "процессов\n\n");

  // Получаем и выводим приоритет системного процесса
  printf("------Системный процесс------\n");
  print_priority("Системный процесс", system_process_pid);

  pid_t old_priority = getpriority(PRIO_PROCESS, system_process_pid);
  // Пытаемся изменить приоритет системного процесса
  printf("\nПытаемся изменить приоритет системного процесса (PID: %d) увеличим на  10...\n",
         system_process_pid);
  if (setpriority(PRIO_PROCESS, system_process_pid, old_priority+10) == -1) {
    perror("Ошибка setpriority для системного процесса (вероятно, из-за "
           "отсутствия прав)");
  } else {
    printf("Приоритет системного процесса изменен.\n");
  }

  // Проверяем приоритет системного процесса после попытки изменения
  print_priority("Приоритет системного процесса после изменения",
                 system_process_pid);
  // Возвращаем старый приоритет
  setpriority(PRIO_PROCESS, system_process_pid, old_priority);              

    printf("\n------Пользовательский процесс------\n");
    print_priority("Пользовательский процесс до изменения приоритета", getpid());

    // Пытаемся увеличить приоритет (уменьшить значение nice)
    printf("\nПытаемся установить приоритет на -10...\n");
    if (setpriority(PRIO_PROCESS, getpid(), -10) == -1) {
      perror("Ошибка setpriority (вероятно, из-за отсутствия прав)");
    }

    print_priority("Пользовательский процесс после изменения приоритета", getpid());

  printf("\n=== Завершение программы ===\n");
  return 0;
}