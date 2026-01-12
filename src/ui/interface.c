#include <stdio.h>
#include <string.h>
#include "interface.h"

// Вывод главного меню
void show_main_menu(void)
{
    printf("\n=== Text File Analyzer ===\n");
    printf("1. Подсчет статистики файла\n");
    printf("2. Поиск слова в файле\n");
    printf("3. Выход\n");
    printf("Выберите действие (1-3): ");
}

// Запрос имени файла
void get_filename_from_user(char* buffer, int size)
{
    printf("Введите путь к файлу: ");
    fgets(buffer, size, stdin);
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
    {
        buffer[len - 1] = '\0';
    }
}

// Запрос слова для поиска
void get_word_from_user(char* buffer, int size)
{
    printf("Введите слово для поиска: ");
    fgets(buffer, size, stdin);
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
    {
        buffer[len - 1] = '\0';
    }
}