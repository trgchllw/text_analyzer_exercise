#ifndef ANALYZER_H
#define ANALYZER_H

// Функция подсчета статистики файла
// Принимает: путь к файлу, указатели на переменные для строк, слов, символов
// Возвращает: 0 в случае успеха, -1 в случае ошибки
int count_file_stats(const char* filename, int* lines, int* words, int* chars);

// Функция поиска слова в файле
// Принимает: путь к файлу, искомое слово
// Возвращает: количество найденных вхождений, -1 в случае ошибки
int search_word_in_file(const char* filename, const char* word);

#endif