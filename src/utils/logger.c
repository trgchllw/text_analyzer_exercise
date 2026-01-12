#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include "logger.h"

char* create_log_filename(void)
{
    char* log_path = malloc(256);
    if (log_path == NULL) return NULL;
    
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    
    struct stat st;
    if (stat("/var/log/text_analyzer", &st) != 0)
    {
        mkdir("/var/log/text_analyzer", 0755);
    }
    
    sprintf(log_path, "/var/log/text_analyzer/%04d-%02d-%02d_%02d-%02d-%02d.log",
            tm_info->tm_year + 1900,
            tm_info->tm_mon + 1,
            tm_info->tm_mday,
            tm_info->tm_hour,
            tm_info->tm_min,
            tm_info->tm_sec);
    
    return log_path;
}

void log_action(const char* user_id, const char* action, const char* result)
{
    char* log_file = create_log_filename();
    if (log_file == NULL) return;
    
    FILE* file = fopen(log_file, "a");
    if (file == NULL)
    {
        free(log_file);
        return;
    }
    
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    
    fprintf(file, "[%04d-%02d-%02d %02d:%02d:%02d] %s: %s = %s\n",
            tm_info->tm_year + 1900,
            tm_info->tm_mon + 1,
            tm_info->tm_mday,
            tm_info->tm_hour,
            tm_info->tm_min,
            tm_info->tm_sec,
            user_id,
            action,
            result);
    
    fclose(file);
    free(log_file);
}