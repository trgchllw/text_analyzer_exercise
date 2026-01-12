#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "analyzer.h"

// Функция подсчета статистики файла
int count_file_stats(const char* filename, int* lines, int* words, int* chars)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        return -1;
    }

    *lines = 0;
    *words = 0;
    *chars = 0;

    char buffer[1024];
    
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        (*lines)++;
        
        int line_length = strlen(buffer);
        *chars += line_length;
        
        int in_word = 0;
        for (int i = 0; i < line_length; i++)
        {

            if (!isspace(buffer[i]))
            {
                if (!in_word)
                {
                    in_word = 1;
                    (*words)++;
                }
            }
            else
            {
                in_word = 0;
            }
        }
    }

    fclose(file);
    return 0;
}

// Функция поиска слова в файле
int search_word_in_file(const char* filename, const char* word)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        return -1;
    }

    int count = 0;      
    char buffer[1024];  
    int word_len = strlen(word);  

    
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        char* pos = buffer;
        
        while ((pos = strstr(pos, word)) != NULL)
        {
            count++;
            pos += word_len;
        }
    }

    fclose(file);
    return count;
}