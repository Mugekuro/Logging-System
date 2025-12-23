#ifndef LOGGING_H
#define LOGGING_H

#include "data_structures.h"

// Logging functions
ErrorCode init_logging_system();
ErrorCode log_action(const char* user_id, const char* module, const char* action, const char* details);
ErrorCode log_error_action(const char* user_id, const char* module, const char* action, const char* details);
ErrorCode get_logs_by_date(const char* date, LogEntry** logs, int* count);
ErrorCode get_logs_by_user(const char* user_id, LogEntry** logs, int* count);
ErrorCode get_logs_by_module(const char* module, LogEntry** logs, int* count);
void cleanup_logging_system();

// Log level management
typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO = 1,
    LOG_LEVEL_WARNING = 2,
    LOG_LEVEL_ERROR = 3,
    LOG_LEVEL_CRITICAL = 4
} LogLevel;

ErrorCode set_log_level(LogLevel level);
LogLevel get_log_level();

#endif // LOGGING_H