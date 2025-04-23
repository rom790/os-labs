#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

void fork_file_demo() {
    printf("\n=== Демонстрация fork() ===\n");
    fflush(stdout);
    const char* filename = "fork.txt";
    
    // Родитель открывает файл
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }

    // Родитель пишет первую строку
    const char* parent_msg1 = "[Родитель] Первая запись (до fork)\n";
    write(fd, parent_msg1, strlen(parent_msg1));
    printf("Родитель записал первую строку\n");
    fflush(stdout);

    // Создаем потомка
    pid_t pid = fork();
    if (pid == -1) {
        perror("Ошибка fork");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Код потомка
        const char* child_msg = "[Потомок] Запись из дочернего процесса\n";
        write(fd, child_msg, strlen(child_msg));
        printf("Потомок записал данные\n");
        fflush(stdout);
        close(fd);
        exit(EXIT_SUCCESS);
    } else {
        // Код родителя
        wait(NULL); // Ждем завершения потомка
        
        // Родитель пишет финальную строку
        const char* parent_msg2 = "[Родитель] Финальная запись (после fork)\n";
        write(fd, parent_msg2, strlen(parent_msg2));
        printf("Родитель записал финальную строку\n");
        fflush(stdout);
        
        close(fd);
        printf("Файл закрыт. Результаты сохранены в %s\n", filename);
        fflush(stdout);
    }
}

void exec_file_demo() {
    printf("\n=== Демонстрация exec() ===\n");
    fflush(stdout);
    const char* filename = "exec.txt";
    
    // Родитель открывает файл
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }

    // Родитель пишет первую строку
    const char* parent_msg1 = "[Родитель] Первая запись (до exec)\n";
    write(fd, parent_msg1, strlen(parent_msg1));
    printf("Родитель записал первую строку\n");
    fflush(stdout);

    // Создаем потомка
    pid_t pid = fork();
    if (pid == -1) {
        perror("Ошибка fork");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Перенаправляем дескриптор
        dup2(fd, 100); // Используем высокий номер для демонстрации
        close(fd);
        
        // Запускаем функцию как отдельную программу
        char fd_str[10];
        snprintf(fd_str, sizeof(fd_str), "%d", 100);
        fflush(stdout);
        execl("./src/6/6.1-ch", "./src/6/6.1-ch", fd_str, NULL);
        perror("Ошибка exec");
        exit(EXIT_FAILURE);
    } else {
        // Код родителя
        wait(NULL); // Ждем завершения потомка
        
        // Родитель пишет финальную строку
        const char* parent_msg2 = "[Родитель] Финальная запись (после exec)\n";
        write(fd, parent_msg2, strlen(parent_msg2));
        printf("Родитель записал финальную строку\n");
        fflush(stdout);
        
        close(fd);
        printf("Файл закрыт. Результаты сохранены в %s\n", filename);
        fflush(stdout);
    }
}

int main() {
    printf("\n6.1 Наследование файловых дескрипторов\n");
    fork_file_demo();
    exec_file_demo();
    return 0;
}