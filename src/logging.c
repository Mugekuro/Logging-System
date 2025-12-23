#include "logging.h"
#include "file_io.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Global variable for log level
static LogLevel current_log_level = LOG_LEVEL_INFO;

// Function to initialize the logging system
ErrorCode init_logging_system() {
    // Ensure logs directory exists
    ErrorCode dir_result = ensure_directory_exists("logs");
    if (dir_result != SUCCESS) {
        return dir_result;
    }

    // Create initial log file if it doesn't exist
    FILE* log_file = fopen("logs/system.log", "a");
    if (log_file) {
        time_t now;
        time(&now);
        char time_str[30];
        format_timestamp(now, time_str, sizeof(time_str));

        fprintf(log_file, "[%s] LOGGING SYSTEM INITIALIZED\n", time_str);
        fclose(log_file);
    } else {
        return ERROR_FILE_OPERATION;
    }

    return SUCCESS;
}

// Function to log an action
ErrorCode log_action(const char* user_id, const char* module, const char* action, const char* details) {
    if (!user_id || !module || !action) {
        return ERROR_INVALID_INPUT;
    }

    // Create log entry
    LogEntry log_entry;
    generate_unique_id(log_entry.logID, sizeof(log_entry.logID), "LOG");

    safe_strcpy(log_entry.userID, user_id, sizeof(log_entry.userID));
    safe_strcpy(log_entry.module, module, sizeof(log_entry.module));
    safe_strcpy(log_entry.action, action, sizeof(log_entry.action));

    if (details) {
        safe_strcpy(log_entry.details, details, sizeof(log_entry.details));
    } else {
        strcpy(log_entry.details, "");
    }

    log_entry.timestamp = time(NULL);

    // Save to binary file using file_io module
    ErrorCode result = save_log(&log_entry);
    if (result != SUCCESS) {
        return result;
    }

    // Also write to text log file for easier reading
    FILE* text_log = fopen("logs/system.log", "a");
    if (text_log) {
        char time_str[30];
        format_timestamp(log_entry.timestamp, time_str, sizeof(time_str));

        fprintf(text_log, "[%s] USER: %s | MODULE: %s | ACTION: %s | DETAILS: %s\n",
                time_str, user_id, module, action, details ? details : "");
        fclose(text_log);
    }

    return SUCCESS;
}

// Function to log an error action
ErrorCode log_error_action(const char* user_id, const char* module, const char* action, const char* details) {
    if (!user_id || !module || !action) {
        return ERROR_INVALID_INPUT;
    }

    // Create log entry
    LogEntry log_entry;
    generate_unique_id(log_entry.logID, sizeof(log_entry.logID), "ERR");

    safe_strcpy(log_entry.userID, user_id, sizeof(log_entry.userID));
    safe_strcpy(log_entry.module, module, sizeof(log_entry.module));
    safe_strcpy(log_entry.action, action, sizeof(log_entry.action));

    if (details) {
        safe_strcpy(log_entry.details, details, sizeof(log_entry.details));
    } else {
        strcpy(log_entry.details, "");
    }

    log_entry.timestamp = time(NULL);

    // Save to binary file using file_io module
    ErrorCode result = save_log(&log_entry);
    if (result != SUCCESS) {
        return result;
    }

    // Also write to error log file
    FILE* error_log = fopen("logs/error.log", "a");
    if (error_log) {
        char time_str[30];
        format_timestamp(log_entry.timestamp, time_str, sizeof(time_str));

        fprintf(error_log, "[%s] ERROR - USER: %s | MODULE: %s | ACTION: %s | DETAILS: %s\n",
                time_str, user_id, module, action, details ? details : "");
        fclose(error_log);
    }

    return SUCCESS;
}

// Function to get logs by date
ErrorCode get_logs_by_date(const char* date, LogEntry** logs, int* count) {
    if (!date || !logs || !count) {
        return ERROR_INVALID_INPUT;
    }

    // First, count matching logs
    LogEntry temp_log;
    FILE* file = fopen("data/logs.dat", "rb");
    if (!file) {
        *logs = NULL;
        *count = 0;
        return SUCCESS;  // No logs file is not an error
    }

    int matching_count = 0;
    while (fread(&temp_log, sizeof(LogEntry), 1, file) == 1) {
        char date_str[11];
        strftime(date_str, sizeof(date_str), "%Y-%m-%d", localtime(&temp_log.timestamp));
        if (strcmp(date_str, date) == 0) {
            matching_count++;
        }
    }
    fclose(file);

    if (matching_count == 0) {
        *logs = NULL;
        *count = 0;
        return SUCCESS;
    }

    // Allocate memory for matching logs
    LogEntry* matching_logs = (LogEntry*)safe_malloc(matching_count * sizeof(LogEntry));
    if (!matching_logs) {
        return ERROR_MEMORY_ALLOCATION;
    }

    // Read matching logs again
    file = fopen("data/logs.dat", "rb");
    if (!file) {
        safe_free((void**)&matching_logs);
        return ERROR_FILE_OPERATION;
    }

    int index = 0;
    while (fread(&temp_log, sizeof(LogEntry), 1, file) == 1) {
        char date_str[11];
        strftime(date_str, sizeof(date_str), "%Y-%m-%d", localtime(&temp_log.timestamp));
        if (strcmp(date_str, date) == 0) {
            matching_logs[index] = temp_log;
            index++;
        }
    }
    fclose(file);

    *logs = matching_logs;
    *count = matching_count;
    return SUCCESS;
}

// Function to get logs by user
ErrorCode get_logs_by_user(const char* user_id, LogEntry** logs, int* count) {
    if (!user_id || !logs || !count) {
        return ERROR_INVALID_INPUT;
    }

    // First, count matching logs
    LogEntry temp_log;
    FILE* file = fopen("data/logs.dat", "rb");
    if (!file) {
        *logs = NULL;
        *count = 0;
        return SUCCESS;  // No logs file is not an error
    }

    int matching_count = 0;
    while (fread(&temp_log, sizeof(LogEntry), 1, file) == 1) {
        if (strcmp(temp_log.userID, user_id) == 0) {
            matching_count++;
        }
    }
    fclose(file);

    if (matching_count == 0) {
        *logs = NULL;
        *count = 0;
        return SUCCESS;
    }

    // Allocate memory for matching logs
    LogEntry* matching_logs = (LogEntry*)safe_malloc(matching_count * sizeof(LogEntry));
    if (!matching_logs) {
        return ERROR_MEMORY_ALLOCATION;
    }

    // Read matching logs again
    file = fopen("data/logs.dat", "rb");
    if (!file) {
        safe_free((void**)&matching_logs);
        return ERROR_FILE_OPERATION;
    }

    int index = 0;
    while (fread(&temp_log, sizeof(LogEntry), 1, file) == 1) {
        if (strcmp(temp_log.userID, user_id) == 0) {
            matching_logs[index] = temp_log;
            index++;
        }
    }
    fclose(file);

    *logs = matching_logs;
    *count = matching_count;
    return SUCCESS;
}

// Function to get logs by module
ErrorCode get_logs_by_module(const char* module, LogEntry** logs, int* count) {
    if (!module || !logs || !count) {
        return ERROR_INVALID_INPUT;
    }

    // First, count matching logs
    LogEntry temp_log;
    FILE* file = fopen("data/logs.dat", "rb");
    if (!file) {
        *logs = NULL;
        *count = 0;
        return SUCCESS;  // No logs file is not an error
    }

    int matching_count = 0;
    while (fread(&temp_log, sizeof(LogEntry), 1, file) == 1) {
        if (strcmp(temp_log.module, module) == 0) {
            matching_count++;
        }
    }
    fclose(file);

    if (matching_count == 0) {
        *logs = NULL;
        *count = 0;
        return SUCCESS;
    }

    // Allocate memory for matching logs
    LogEntry* matching_logs = (LogEntry*)safe_malloc(matching_count * sizeof(LogEntry));
    if (!matching_logs) {
        return ERROR_MEMORY_ALLOCATION;
    }

    // Read matching logs again
    file = fopen("data/logs.dat", "rb");
    if (!file) {
        safe_free((void**)&matching_logs);
        return ERROR_FILE_OPERATION;
    }

    int index = 0;
    while (fread(&temp_log, sizeof(LogEntry), 1, file) == 1) {
        if (strcmp(temp_log.module, module) == 0) {
            matching_logs[index] = temp_log;
            index++;
        }
    }
    fclose(file);

    *logs = matching_logs;
    *count = matching_count;
    return SUCCESS;
}

// Function to set log level
ErrorCode set_log_level(LogLevel level) {
    if (level < LOG_LEVEL_DEBUG || level > LOG_LEVEL_CRITICAL) {
        return ERROR_INVALID_INPUT;
    }

    current_log_level = level;
    return SUCCESS;
}

// Function to get log level
LogLevel get_log_level() {
    return current_log_level;
}

// Function to cleanup the logging system
void cleanup_logging_system() {
    // Log system shutdown
    FILE* log_file = fopen("logs/system.log", "a");
    if (log_file) {
        time_t now;
        time(&now);
        char time_str[30];
        format_timestamp(now, time_str, sizeof(time_str));

        fprintf(log_file, "[%s] LOGGING SYSTEM SHUTDOWN\n", time_str);
        fclose(log_file);
    }
}