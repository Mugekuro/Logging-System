#ifndef CONFIG_H
#define CONFIG_H

// System configuration constants
#define SYSTEM_NAME "College of Technologies - Student Body Organization"
#define MAX_LOGIN_ATTEMPTS 3
#define SESSION_TIMEOUT_SECONDS 1800  // 30 minutes
#define BACKUP_RETENTION_DAYS 30
#define LOG_RETENTION_DAYS 90
#define MAX_RECORDS_PER_PAGE 50
#define DEFAULT_PAYMENT_AMOUNT 500.0f  // Default amount for college shirt
#define DATA_FILE_VERSION 1

// File paths
#define STUDENT_DATA_FILE "../data/students.dat"
#define OFFICER_DATA_FILE "../data/officers.dat"
#define PAYMENT_DATA_FILE "../data/payments.dat"
#define LOG_DATA_FILE "../data/logs.dat"
#define CONFIG_DATA_FILE "../data/config.dat"
#define USER_DATA_FILE "../data/users.dat"

// Backup directories
#define DAILY_BACKUP_DIR "../backup/daily"
#define WEEKLY_BACKUP_DIR "../backup/weekly"
#define MONTHLY_BACKUP_DIR "../backup/monthly"
#define RECOVERY_DIR "../backup/recovery"

// Log file paths
#define ACCESS_LOG_FILE "../logs/access.log"
#define ERROR_LOG_FILE "../logs/error.log"
#define AUDIT_LOG_FILE "../logs/audit.log"
#define SYSTEM_LOG_FILE "../logs/system.log"

// Security settings
#define PASSWORD_MIN_LENGTH 6
#define PASSWORD_MAX_LENGTH 128
#define SESSION_TOKEN_LENGTH 32
#define MAX_FAILED_LOGIN_LOCKOUT_MINUTES 30

#endif // CONFIG_H