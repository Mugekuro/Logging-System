#ifndef BACKUP_SYSTEM_H
#define BACKUP_SYSTEM_H

#include "data_structures.h"
#include <time.h>

// Backup types
typedef enum {
    BACKUP_DAILY,
    BACKUP_WEEKLY,
    BACKUP_MONTHLY,
    BACKUP_MANUAL
} BackupType;

// Backup configuration structure
typedef struct {
    char source_directory[256];
    char destination_directory[256];
    BackupType type;
    int retention_days;
    bool compress_backups;
    bool verify_integrity;
} BackupConfig;

// Backup result structure
typedef struct {
    char backup_filename[256];
    time_t backup_time;
    long file_size;
    bool success;
    char error_message[200];
} BackupResult;

// Backup management functions
ErrorCode init_backup_system(void);
ErrorCode cleanup_backup_system(void);

// Core backup functions
ErrorCode create_backup(const BackupConfig* config, BackupResult* result);
ErrorCode schedule_automatic_backup(BackupType type);
ErrorCode restore_from_backup(const char* backup_filename, const char* restore_directory);

// Backup verification functions
ErrorCode verify_backup_integrity(const char* backup_filename);
ErrorCode list_available_backups(const char* backup_directory, BackupResult* backups, int max_backups, int* actual_count);

// Backup maintenance functions
ErrorCode cleanup_old_backups(const char* backup_directory, int retention_days);
ErrorCode get_backup_statistics(int* total_backups, long* total_size);

// Recovery functions
ErrorCode restore_student_data(const char* backup_filename);
ErrorCode restore_officer_data(const char* backup_filename);
ErrorCode restore_payment_data(const char* backup_filename);
ErrorCode restore_all_data(const char* backup_filename);

#endif // BACKUP_SYSTEM_H