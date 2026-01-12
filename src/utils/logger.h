#ifndef LOGGER_H
#define LOGGER_H

void log_action(const char* user_id, const char* action, const char* result);
char* create_log_filename(void);

#endif