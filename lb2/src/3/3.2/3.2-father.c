#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main() {
  int i, ppid, pid[6], status[5], result[5];
  char *son[] = {"3.2-son1", "3.2-son2", "3.2-son3", "3.2-son4"};
  char *sonPath[] = {"./src/3/3.2/3.2-son1", "./src/3/3.2/3.2-son2",
                     "./src/3/3.2/3.2-son3", "./src/3/3.2/3.2-son4"};
  int option[] = {WNOHANG, WUNTRACED, WNOHANG, WNOHANG};

  pid[5] = getpid();
  ppid = getppid();
  printf("Параметры родительского процесса: pid=%i ppid=%i\n", pid[5], ppid);
  fflush(stdout);
  for (i = 0; i < 4; i++) {
    if ((pid[i] = fork()) == 0) {
      execl(sonPath[i], son[i], NULL);
    }
  }

  system("ps -H f > file.txt");
  for (i = 0; i < 4; i++) {
    result[i] = waitpid(pid[i], &status[i], option[i]);
    printf("%d) Дочерний процесс с PID = %d завершился со статусом %d\n", (1 + i),
           result[i], status[i]);
           fflush(stdout);
  }

  for (i = 0; i < 4; i++)
    if (WIFEXITED(status[i]) == 0) {
      printf("Процесс с pid = %d завершился с ошибкой.\n", pid[i]);
      fflush(stdout);
    } else {
      printf("Процесс с pid = %d завершился корректно.\n", pid[i]);
      fflush(stdout);
    }
  return 0;
}