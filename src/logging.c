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

// Log a message with a specific log level
ErrorCode log_message(LogLevel level, const char *module, const char *function, const char *message) {
    if (module == NULL || function == NULL || message == NULL) {
        return ERROR_INVALID_INPUT;
    }
    
    // Format the action based on log level
    char action[100];
    switch (level) {
        case LOG_LEVEL_DEBUG:
            snprintf(action, sizeof(action), "DEBUG: %s", message);
            break;
        case LOG_LEVEL_INFO:
            snprintf(action, sizeof(action), "INFO: %s", message);
            break;
        case LOG_LEVEL_WARNING:
            snprintf(action, sizeof(action), "WARNING: %s", message);
            break;
        case LOG_LEVEL_ERROR:
            snprintf(action, sizeof(action), "ERROR: %s", message);
            break;
        case LOG_LEVEL_CRITICAL:
            snprintf(action, sizeof(action), "CRITICAL: %s", message);
            break;
        default:
            snprintf(action, sizeof(action), "UNKNOWN: %s", message);
            break;
    }
    
    // Format the details
    char details[200];
    snprintf(details, sizeof(details), "In %s::%s", module, function);
    
    // Log the event
    return log_event(NULL, action, module, details);
}

// Get all log entries from the system
ErrorCode get_all_logs(LogEntry *logs, int max_count, int *actual_count) {
    if (logs == NULL || actual_count == NULL) {
        return ERROR_INVALID_INPUT;
    }
    
    return read_all_logs(logs, max_count, actual_count);
}

// Search logs by user ID
ErrorCode search_logs_by_user(const char *user_id, LogEntry *results, int max_results, int *actual_count) {
    if (user_id == NULL || results == NULL || actual_count == NULL) {
        return ERROR_INVALID_INPUT;
    }
    
    *actual_count = 0;
    
    LogEntry all_logs[MAX_LOGS];
    int total_count = 0;
    ErrorCode read_result = read_all_logs(all_logs, MAX_LOGS, &total_count);
    if (read_result != SUCCESS) {
        return read_result;
    }
    
    // Search for logs with matching user ID
    for (int i = 0; i < total_count && *actual_count < max_results; i++) {
        if (strcmp(all_logs[i].userID, user_id) == 0) {
            results[*actual_count] = all_logs[i];
            (*actual_count)++;
        }
    }
    
    return SUCCESS;
}

// Search logs by module
ErrorCode search_logs_by_module(const char *module, LogEntry *results, int max_results, int *actual_count) {
    if (module == NULL || results == NULL || actual_count == NULL) {
        return ERROR_INVALID_INPUT;
    }
    
    *actual_count = 0;
    
    LogEntry all_logs[MAX_LOGS];
    int total_count = 0;
    ErrorCode read_result = read_all_logs(all_logs, MAX_LOGS, &total_count);
    if (read_result != SUCCESS) {
        return read_result;
    }
    
    // Search for logs with matching module
    for (int i = 0; i < total_count && *actual_count < max_results; i++) {
        if (strcmp(all_logs[i].module, module) == 0) {
            results[*actual_count] = all_logs[i];
            (*actual_count)++;
        }
    }
    
    return SUCCESS;
}

// Search logs by action
ErrorCode search_logs_by_action(const char *action, LogEntry *results, int max_results, int *actual_count) {
    if (action == NULL || results == NULL || actual_count == NULL) {
        return ERROR_INVALID_INPUT;
    }
    
    *actual_count = 0;
    
    LogEntry all_logs[MAX_LOGS];
    int total_count = 0;
    ErrorCode read_result = read_all_logs(all_logs, MAX_LOGS, &total_count);
    if (read_result != SUCCESS) {
        return read_result;
    }
    
    // Search for logs with matching action
    for (int i = 0; i < total_count && *actual_count < max_results; i++) {
        if (strstr(all_logs[i].action, action) != NULL) {
            results[*actual_count] = all_logs[i];
            (*actual_count)++;
        }
    }
    
    return SUCCESS;
}

// Search logs by date range
ErrorCode search_logs_by_date_range(time_t start_date, time_t end_date, LogEntry *results, int max_results, int *actual_count) {
    if (results == NULL || actual_count == NULL) {
        return ERROR_INVALID_INPUT;
    }
    
    *actual_count = 0;
    
    LogEntry all_logs[MAX_LOGS];
    int total_count = 0;
    ErrorCode read_result = read_all_logs(all_logs, MAX_LOGS, &total_count);
    if (read_result != SUCCESS) {
        return read_result;
    }
    
    // Search for logs within the specified date range
    for (int i = 0; i < total_count && *actual_count < max_results; i++) {
        if (all_logs[i].timestamp >= start_date && all_logs[i].timestamp <= end_date) {
            results[*actual_count] = all_logs[i];
            (*actual_count)++;
        }
    }
    
    return SUCCESS;
}

// Get log count
int get_log_count(void) {
    LogEntry logs[MAX_LOGS];
    int actual_count = 0;
    
    ErrorCode result = read_all_logs(logs, MAX_LOGS, &actual_count);
    if (result != SUCCESS) {
        return 0;
    }
    
    return actual_count;
}

// Clear old logs based on retention policy
ErrorCode clear_old_logs(int days_to_keep) {
    if (days_to_keep <= 0) {
        return ERROR_INVALID_INPUT;
    }
    
    // Calculate the cutoff date
    time_t cutoff_date = get_current_time() - (days_to_keep * 24 * 60 * 60);
    
    // Read all logs
    LogEntry all_logs[MAX_LOGS];
    int total_count = 0;
    ErrorCode read_result = read_all_logs(all_logs, MAX_LOGS, &total_count);
    if (read_result != SUCCESS) {
        return read_result;
    }
    
    // Filter out logs that are too old
    LogEntry kept_logs[MAX_LOGS];
    int kept_count = 0;
    
    for (int i = 0; i < total_count; i++) {
        if (all_logs[i].timestamp >= cutoff_date) {
            kept_logs[kept_count] = all_logs[i];
            kept_count++;
        }
    }
    
    // Write the kept logs back to the file
    FILE *file = fopen(LOG_FILE, "wb");
    if (file == NULL) {
        log_error("LOGGING", "clear_old_logs", ERROR_FILE_OPERATION, 
                  "Failed to open log file for writing");
        return ERROR_FILE_OPERATION;
    }
    
    // Write header
    FileHeader header = {0};
    safe_strcpy(header.magic, "LOGS", sizeof(header.magic));
    header.version = FILE_VERSION;
    header.record_count = kept_count;
    header.last_modified = time(NULL);
    
    if (fwrite(&header, sizeof(FileHeader), 1, file) != 1) {
        fclose(file);
        return ERROR_FILE_OPERATION;
    }
    
    // Write all kept records
    for (int i = 0; i < kept_count; i++) {
        if (fwrite(&kept_logs[i], sizeof(LogEntry), 1, file) != 1) {
            fclose(file);
            log_error("LOGGING", "clear_old_logs", ERROR_FILE_OPERATION, 
                      "Failed to write log record during cleanup");
            return ERROR_FILE_OPERATION;
        }
    }
    
    fclose(file);
    
    return SUCCESS;
}

// Export logs to a file in a specific format
ErrorCode export_logs(const char *filename, const char *format) {
    if (filename == NULL || format == NULL) {
        return ERROR_INVALID_INPUT;
    }
    
    // Read all logs
    LogEntry all_logs[MAX_LOGS];
    int total_count = 0;
    ErrorCode read_result = read_all_logs(all_logs, MAX_LOGS, &total_count);
    if (read_result != SUCCESS) {
        return read_result;
    }
    
    FILE *export_file = fopen(filename, "w");
    if (export_file == NULL) {
        log_error("LOGGING", "export_logs", ERROR_FILE_OPERATION, 
                  "Failed to open export file for writing");
        return ERROR_FILE_OPERATION;
    }
    
    if (strcmp(format, "csv") == 0) {
        // Write CSV header
        fprintf(export_file, "LogID,UserID,Action,Module,Timestamp,Details\n");
        
        // Write all logs in CSV format
        for (int i = 0; i < total_count; i++) {
            char timestamp_str[20];
            format_timestamp(all_logs[i].timestamp, timestamp_str, sizeof(timestamp_str));
            
            fprintf(export_file, "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
                    all_logs[i].logID, all_logs[i].userID, all_logs[i].action,
                    all_logs[i].module, timestamp_str, all_logs[i].details);
        }
    } else if (strcmp(format, "json") == 0) {
        // Write JSON array start
        fprintf(export_file, "[\n");
        
        // Write all logs in JSON format
        for (int i = 0; i < total_count; i++) {
            char timestamp_str[20];
            format_timestamp(all_logs[i].timestamp, timestamp_str, sizeof(timestamp_str));
            
            fprintf(export_file, "  {\n");
            fprintf(export_file, "    \"logID\": \"%s\",\n", all_logs[i].logID);
            fprintf(export_file, "    \"userID\": \"%s\",\n", all_logs[i].userID);
            fprintf(export_file, "    \"action\": \"%s\",\n", all_logs[i].action);
            fprintf(export_file, "    \"module\": \"%s\",\n", all_logs[i].module);
            fprintf(export_file, "    \"timestamp\": \"%s\",\n", timestamp_str);
            fprintf(export_file, "    \"details\": \"%s\"\n", all_logs[i].details);
            fprintf(export_file, "  }%s\n", (i < total_count - 1) ? "," : "");
        }
        
        // Write JSON array end
        fprintf(export_file, "]\n");
    } else if (strcmp(format, "txt") == 0) {
        // Write all logs in text format
        for (int i = 0; i < total_count; i++) {
            char timestamp_str[20];
            format_timestamp(all_logs[i].timestamp, timestamp_str, sizeof(timestamp_str));
            
            fprintf(export_file, "[%s] %s - %s::%s - %s - %s\n",
                    timestamp_str, all_logs[i].logID, all_logs[i].userID,
                    all_logs[i].module, all_logs[i].action, all_logs[i].details);
        }
    } else {
        fclose(export_file);
        return ERROR_INVALID_INPUT;
    }
    
    fclose(export_file);
    return SUCCESS;
}

// Validate a log entry structure
ValidationResult validate_log_entry(const LogEntry *log_entry) {
    ValidationResult result = {false, ""};
    
    if (log_entry == NULL) {
        strcpy(result.errorMessage, "LogEntry cannot be NULL");
        return result;
    }
    
    // Validate log ID
    if (strlen(log_entry->logID) == 0) {
        strcpy(result.errorMessage, "Log ID cannot be empty");
        return result;
    }
    
    // Validate action
    if (strlen(log_entry->action) == 0) {
        strcpy(result.errorMessage, "Log action cannot be empty");
        return result;
    }
    
    // Validate module
    if (strlen(log_entry->module) == 0) {
        strcpy(result.errorMessage, "Log module cannot be empty");
        return result;
    }
    
    // Validate timestamp
    ValidationResult date_result = validate_date(log_entry->timestamp);
    if (!date_result.isValid) {
        snprintf(result.errorMessage, sizeof(result.errorMessage), 
                 "Invalid log timestamp: %s", date_result.errorMessage);
        return result;
    }
    
    // If we got here, the log entry is valid
    result.isValid = true;
    return result;
}

// Clean up the logging system
void logging_cleanup(void) {
    // In this implementation, there's no dynamic allocation to clean up
    // but in a more complex system, you might have caches or other resources to free
}