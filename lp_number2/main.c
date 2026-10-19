#include "solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    FILE* file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return EXIT_FAILURE;
    }


    int capacity = 16;
    int length = 0;
    char* buffer = (char*)malloc(capacity * sizeof(char));
    if (!buffer) {
        fclose(file);
        return 1;
    }

    int c;
    while (1) {
        length = 0;

        while ((c = fgetc(file)) != '\n' && c != EOF) {
            if (c == '\r') {
                continue;
            }

            if (length + 1 >= capacity) {
                capacity *= 2;
                char* temp = (char*)realloc(buffer, capacity * sizeof(char));
                if (!temp) {
                    free(buffer);
                    fclose(file);
                    return 1;
                }
                buffer = temp;
            }

            buffer[length++] = (char)c;
        }

        buffer[length] = '\0';
        if (length > 0) {
            printf("\nИсходный ребус: %s\n", buffer);
            str_pars puzzle;
            parse(buffer, &puzzle, length);
            clock_t start = clock();
            solve_base(&puzzle);
            clock_t end = clock();
            double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
            printf("Время решения: %f сек.\n", time_spent);
            free_struct(&puzzle);
        }

        if (c == EOF) {
            break;
        }
    }

    free(buffer);
    fclose(file);

    return 0;
}