

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int fd = atoi(argv[1]);
    const char* msg = "[Exec-Потомок] Запись через exec\n";
    write(fd, msg, strlen(msg));
    printf("Exec-потомок записал строку\n");
    return 0;
}