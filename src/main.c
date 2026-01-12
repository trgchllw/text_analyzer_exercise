#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "core/analyzer.h"
#include "ui/interface.h"
#include "utils/logger.h"

// Глобальные переменные для аргументов
char* config_file_path = NULL;  // -C
char* input_file_path = NULL;   // -f
char* output_file_path = NULL;  // -o
int debug_mode = 0;             // -d
int silent_mode = 0;            // -S
int keep_temp = 0;              // -t

time_t start_time, end_time;

char user_id[256] = "unknown_user";

void parse_arguments(int argc, char* argv[]);
void show_author_info(void);
void show_help(const char* program_name);
void write_log_with_duration(const char* action, const char* result, int exit_code);

int main(int argc, char* argv[])
{
    show_author_info();
    
    start_time = time(NULL);
    
    parse_arguments(argc, argv);
    
    if (input_file_path != NULL)
    {
        int lines, words, chars;
        int result = count_file_stats(input_file_path, &lines, &words, &chars);
        
        if (result == 0)
        {
            printf("Строк: %d, Слов: %d, Символов: %d\n", lines, words, chars);
            
            write_log_with_duration("batch_stats", "SUCCESS", 0);
            
            if (output_file_path != NULL)
            {
                FILE* out = fopen(output_file_path, "w");
                if (out != NULL)
                {
                    fprintf(out, "Строк: %d\nСлов: %d\nСимволов: %d\n", lines, words, chars);
                    fclose(out);
                    printf("Результат сохранен в %s\n", output_file_path);
                }
            }
        }
        else
        {
            write_log_with_duration("batch_stats", "ERROR_FILE_NOT_FOUND", -1);
            return -1;
        }
    }
    else  // Интерактивное меню
    {
        while (1)
        {
            show_main_menu();
            
            char choice[10];
            fgets(choice, sizeof(choice), stdin);
            
            if (choice[0] == '1')
            {
                char filename[256];
                get_filename_from_user(filename, sizeof(filename));
                
                int lines, words, chars;
                int result = count_file_stats(filename, &lines, &words, &chars);
                
                if (result == 0)
                {
                    printf("\nСтатистика файла '%s':\n", filename);
                    printf("Строк: %d\n", lines);
                    printf("Слов: %d\n", words);
                    printf("Символов: %d\n", chars);
                    
                    // Запись лога
                    char log_result[100];
                    sprintf(log_result, "Stats: L=%d W=%d C=%d", lines, words, chars);
                    write_log_with_duration("count_stats", log_result, 0);
                }
                else
                {
                    printf("Ошибка: не удалось открыть файл\n");
                    write_log_with_duration("count_stats", "ERROR_FILE_NOT_FOUND", -1);
                }
            }
            else if (choice[0] == '2')
            {
                char filename[256];
                get_filename_from_user(filename, sizeof(filename));
                
                char word[100];
                get_word_from_user(word, sizeof(word));
                
                int count = search_word_in_file(filename, word);
                
                if (count >= 0)
                {
                    printf("Слово '%s' найдено %d раз(а)\n", word, count);
                    
                    char log_result[100];
                    sprintf(log_result, "Found %d times", count);
                    write_log_with_duration("search_word", log_result, 0);
                }
                else
                {
                    printf("Ошибка: не удалось открыть файл\n");
                    write_log_with_duration("search_word", "ERROR_FILE_NOT_FOUND", -1);
                }
            }
            else if (choice[0] == '3')
            {
                printf("До свидания!\n");
                write_log_with_duration("program_exit", "USER_EXIT", 0);
                break;
            }
            else
            {
                printf("Неверный выбор.\n");
            }
        }
    }
    
    return 0;
}


void parse_arguments(int argc, char* argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "C:df:hmio:St")) != -1)
    {
        switch (opt)
        {
            case 'C':
                config_file_path = optarg;
                printf("Конфиг: %s\n", config_file_path);
                break;
            case 'd':
                debug_mode = 1;
                break;
            case 'f':
                input_file_path = optarg;
                printf("Файл: %s\n", input_file_path);
                break;
            case 'h':
                show_help(argv[0]);
                exit(0);
                break;
            case 'i':
                show_author_info();
                exit(0);
                break;
            case 'o':
                output_file_path = optarg;
                printf("Выход: %s\n", output_file_path);
                break;
            case 'S':
                silent_mode = 1;
                break;
            case 't':
                keep_temp = 1;
                break;
        }
    }
}

void show_author_info(void)
{
    printf("=====================================\n");
    printf("Text Analyzer\n");
    printf("Автор: Березин Евгений Андреевич\n");
    printf("Группа: ИТПМ-125\n");
    printf("Преподаватель: Мокряков Алексей Викторович\n");
    printf("=====================================\n\n");
}

void show_help(const char* program_name)
{
    printf("ИСПОЛЬЗОВАНИЕ: %s [ОПЦИИ]\n", program_name);
    printf("  -C <файл>  Конфигурационный файл\n");
    printf("  -d         Режим отладки\n");
    printf("  -f <файл>  Входной файл\n");
    printf("  -h         Справка\n");
    printf("  -i         Информация об авторе\n");
    printf("  -o <файл>  Выходной файл\n");
    printf("  -S         Тихий режим\n");
    printf("  -t         Не удалять временные файлы\n");
}

void write_log_with_duration(const char* action, const char* result, int exit_code)
{
    if (silent_mode) return;
    
    end_time = time(NULL);
    int duration = (int)difftime(end_time, start_time);
    
    char full_result[512];
    sprintf(full_result, "%s | Duration=%ds | Code=%d", result, duration, exit_code);
    
    log_action(user_id, action, full_result);
}