#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (putchar(ch) == EOF) {
            perror("Ошибка вывода");
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    if (ferror(file)) {
        perror("Ошибка чтения файла");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    return 0;
}