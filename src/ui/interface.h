#ifndef INTERFACE_H
#define INTERFACE_H

// Функция вывода главного меню
void show_main_menu(void);

// Функция запроса имени файла от пользователя
void get_filename_from_user(char* buffer, int size);

// Функция запроса слова для поиска
void get_word_from_user(char* buffer, int size);

#endif