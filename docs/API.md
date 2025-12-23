# API Documentation

This document provides detailed information about the APIs and functions available in the Student Body Organization Management System.

## Table of Contents
- [Data Structures](#data-structures)
- [Utility Functions](#utility-functions)
- [Authentication API](#authentication-api)
- [Student Management API](#student-management-api)
- [Officer Management API](#officer-management-api)
- [Payment Processing API](#payment-processing-api)
- [Logging API](#logging-api)
- [File I/O API](#file-io-api)
- [UI API](#ui-api)

## Data Structures

### Student
```c
typedef struct {
    char studentID[MAX_ID_LENGTH];        // Unique identifier for the student
    char firstName[MAX_FIRST_NAME_LENGTH]; // Student's first name
    char lastName[MAX_LAST_NAME_LENGTH];   // Student's last name
    char fullName[MAX_FULL_NAME_LENGTH];   // Combined first and last name
    char email[MAX_EMAIL_LENGTH];          // Student's email address
    char phone[MAX_PHONE_LENGTH];          // Student's phone number
    time_t registrationDate;               // Date of registration
    char status[MAX_STATUS_LENGTH];        // Active, Inactive, Graduated
} Student;
```

### Officer
```c
typedef struct {
    char officerID[MAX_ID_LENGTH];         // Unique identifier for the officer
    char name[MAX_NAME_LENGTH];            // Officer's full name
    char position[MAX_POSITION_LENGTH];    // Officer's position/title
    int accessLevel;                       // Access level (1-5)
    time_t lastLogin;                      // Timestamp of last login
    char status[MAX_STATUS_LENGTH];        // Active, Inactive
} Officer;
```

### Payment
```c
typedef struct {
    char paymentID[MAX_ID_LENGTH];         // Unique identifier for the payment
    char studentID[MAX_ID_LENGTH];         // ID of the paying student
    float amount;                          // Payment amount
    char purpose[MAX_PURPOSE_LENGTH];      // Description of payment purpose
    time_t paymentDate;                    // Date of payment
    char status[MAX_STATUS_LENGTH];        // Pending, Completed, Failed, Refunded
    char paymentMethod[MAX_PAYMENT_METHOD_LENGTH]; // Cash, Card, Online
} Payment;
```

### LogEntry
```c
typedef struct {
    char logID[MAX_ID_LENGTH];             // Unique identifier for the log entry
    char userID[MAX_ID_LENGTH];            // ID of user who performed action
    char action[MAX_ACTION_LENGTH];        // Description of the action
    char module[MAX_MODULE_LENGTH];        // Module where action occurred
    time_t timestamp;                      // Time when action occurred
    char details[MAX_DETAILS_LENGTH];      // Additional details about the action
} LogEntry;
```

## Utility Functions

### String Utilities
```c
void safe_strcpy(char *dest, const char *src, size_t dest_size);
void safe_strcat(char *dest, const char *src, size_t dest_size);
int safe_strcmp(const char *str1, const char *str2);
size_t safe_strlen(const char *str);
```

### Input Validation Functions
```c
ValidationResult validate_student_id(const char *student_id);
ValidationResult validate_name(const char *name);
ValidationResult validate_email(const char *email);
ValidationResult validate_phone(const char *phone);
ValidationResult validate_amount(float amount);
ValidationResult validate_date(time_t date);
```

### String Sanitization Functions
```c
void sanitize_string(char *str);
char* trim_whitespace(char *str);
```

### Password Utilities
```c
int validate_password_strength(const char *password);
void generate_unique_id(char *id_buffer, size_t buffer_size);
```

### Memory Management Utilities
```c
void* safe_malloc(size_t size);
void safe_free(void *ptr);
```

### Time Utilities
```c
char* format_timestamp(time_t timestamp, char *buffer, size_t buffer_size);
time_t get_current_time();
```

### Error Handling Utilities
```c
void log_error(const char *module, const char *function, int error_code, const char *message);
```

## Authentication API

### Initialization
```c
ErrorCode auth_init(void);
```

### User Management
```c
ErrorCode create_user(const char *username, const char *password, int access_level);
ErrorCode authenticate_user(const char *username, const char *password, User *user);
ErrorCode update_password(const char *session_id, const char *username, const char *new_password);
ErrorCode lock_account(const char *username);
ErrorCode unlock_account(const char *username);
```

### Session Management
```c
ErrorCode create_session(const User *user, Session *session);
bool validate_session(const char *session_id);
ErrorCode end_session(const char *session_id);
bool check_access_level(const char *session_id, int required_level);
```

### Password Management
```c
ErrorCode hash_password(const char *password, char *hash_buffer, size_t buffer_size);
bool verify_password(const char *password, const char *hash);
```

### Account Status
```c
bool is_account_locked(const char *username);
int get_failed_attempts(const char *username);
ErrorCode reset_failed_attempts(const char *username);
```

### Cleanup
```c
void auth_cleanup(void);
```

## Student Management API

### Initialization
```c
ErrorCode student_init(void);
```

### Student Operations
```c
ErrorCode add_student(const Student *student);
ErrorCode find_student(const char *student_id, Student *student);
ErrorCode update_student(const Student *updated_student);
ErrorCode remove_student(const char *student_id);
```

### Student Retrieval
```c
ErrorCode get_all_students(Student *students, int max_count, int *actual_count);
int get_student_count(void);
```

### Student Search
```c
ErrorCode search_students_by_name(const char *name, Student *results, int max_results, int *actual_count);
ErrorCode search_students_by_status(const char *status, Student *results, int max_results, int *actual_count);
```

### Validation
```c
ValidationResult validate_student(const Student *student);
bool student_exists(const char *student_id);
```

### Cleanup
```c
void student_cleanup(void);
```

## Officer Management API

### Initialization
```c
ErrorCode officer_init(void);
```

### Officer Operations
```c
ErrorCode add_officer(const Officer *officer);
ErrorCode find_officer(const char *officer_id, Officer *officer);
ErrorCode update_officer(const Officer *updated_officer);
ErrorCode remove_officer(const char *officer_id);
```

### Officer Retrieval
```c
ErrorCode get_all_officers(Officer *officers, int max_count, int *actual_count);
int get_officer_count(void);
```

### Officer Search
```c
ErrorCode search_officers_by_name(const char *name, Officer *results, int max_results, int *actual_count);
ErrorCode search_officers_by_position(const char *position, Officer *results, int max_results, int *actual_count);
```

### Validation and Access
```c
ValidationResult validate_officer(const Officer *officer);
bool officer_exists(const char *officer_id);
bool officer_has_access_level(const char *officer_id, int required_level);
```

### Cleanup
```c
void officer_cleanup(void);
```

## Payment Processing API

### Initialization
```c
ErrorCode payment_init(void);
```

### Payment Operations
```c
ErrorCode process_payment(Payment *payment);
ErrorCode find_payment(const char *payment_id, Payment *payment);
ErrorCode update_payment(const Payment *updated_payment);
ErrorCode remove_payment(const char *payment_id);
```

### Payment Retrieval
```c
ErrorCode get_all_payments(Payment *payments, int max_count, int *actual_count);
int get_payment_count(void);
```

### Payment Calculations
```c
ErrorCode get_student_total_payments(const char *student_id, float *total_amount);
ErrorCode get_student_payments(const char *student_id, Payment *payments, int max_count, int *actual_count);
```

### Payment Search
```c
ErrorCode search_payments_by_purpose(const char *purpose, Payment *results, int max_results, int *actual_count);
ErrorCode search_payments_by_date_range(time_t start_date, time_t end_date, Payment *results, int max_results, int *actual_count);
ErrorCode calculate_total_payments_in_date_range(time_t start_date, time_t end_date, float *total_amount);
```

### Validation
```c
ValidationResult validate_payment(const Payment *payment);
bool payment_exists(const char *payment_id);
```

### Cleanup
```c
void payment_cleanup(void);
```

## Logging API

### Initialization
```c
ErrorCode logging_init(void);
```

### Event Logging
```c
ErrorCode log_event(const char *user_id, const char *action, const char *module, const char *details);
ErrorCode log_message(LogLevel level, const char *module, const char *function, const char *message);
```

### Log Retrieval
```c
ErrorCode get_all_logs(LogEntry *logs, int max_count, int *actual_count);
int get_log_count(void);
```

### Log Search
```c
ErrorCode search_logs_by_user(const char *user_id, LogEntry *results, int max_results, int *actual_count);
ErrorCode search_logs_by_module(const char *module, LogEntry *results, int max_results, int *actual_count);
ErrorCode search_logs_by_action(const char *action, LogEntry *results, int max_results, int *actual_count);
ErrorCode search_logs_by_date_range(time_t start_date, time_t end_date, LogEntry *results, int max_results, int *actual_count);
```

### Log Management
```c
ErrorCode clear_old_logs(int days_to_keep);
ErrorCode export_logs(const char *filename, const char *format);
```

### Validation
```c
ValidationResult validate_log_entry(const LogEntry *log_entry);
```

### Cleanup
```c
void logging_cleanup(void);
```

## File I/O API

### Initialization
```c
ErrorCode file_io_init(void);
```

### Student File Operations
```c
ErrorCode write_student_record(const Student *student);
ErrorCode read_student_record(const char *student_id, Student *student);
ErrorCode update_student_record(const Student *student);
ErrorCode delete_student_record(const char *student_id);
ErrorCode read_all_students(Student *students, int max_count, int *actual_count);
```

### Officer File Operations
```c
ErrorCode write_officer_record(const Officer *officer);
ErrorCode read_officer_record(const char *officer_id, Officer *officer);
ErrorCode update_officer_record(const Officer *officer);
ErrorCode delete_officer_record(const char *officer_id);
ErrorCode read_all_officers(Officer *officers, int max_count, int *actual_count);
```

### Payment File Operations
```c
ErrorCode write_payment_record(const Payment *payment);
ErrorCode read_payment_record(const char *payment_id, Payment *payment);
ErrorCode update_payment_record(const Payment *payment);
ErrorCode delete_payment_record(const char *payment_id);
ErrorCode read_all_payments(Payment *payments, int max_count, int *actual_count);
```

### Log File Operations
```c
ErrorCode write_log_record(const LogEntry *log_entry);
ErrorCode read_all_logs(LogEntry *logs, int max_count, int *actual_count);
```

### User File Operations
```c
ErrorCode write_user_record(const User *user);
ErrorCode read_user_record(const char *username, User *user);
ErrorCode update_user_record(const User *user);
ErrorCode delete_user_record(const char *username);
ErrorCode read_all_users(User *users, int max_count, int *actual_count);
```

### Backup and Recovery
```c
ErrorCode create_backup(const char *backup_type);
ErrorCode restore_from_backup(const char *backup_type);
```

### Data Integrity
```c
ErrorCode validate_file_integrity(const char *filename);
```

### Cleanup
```c
void file_io_cleanup(void);
```

## UI API

### Initialization
```c
ErrorCode ui_init(void);
```

### Screen Management
```c
void clear_screen(void);
void display_header(const char *title);
void display_message(const char *message, const char *type);
```

### Input Handling
```c
ErrorCode get_user_input(const char *prompt, char *input_buffer, size_t buffer_size, 
                         ValidationResult (*validation_func)(const char*));
char display_menu(const char *title, const MenuOption *options, int option_count, const char *session_id);
```

### Data Display
```c
void display_students(const Student *students, int count);
void display_student(const Student *student);
void display_officers(const Officer *officers, int count);
void display_officer(const Officer *officer);
void display_payments(const Payment *payments, int count);
void display_payment(const Payment *payment);
void display_logs(const LogEntry *logs, int count);
void display_table(const char **headers, const char ***data, int row_count, int col_count);
```

### Menu Systems
```c
ErrorCode display_login(Session *session);
ErrorCode display_logout(const char *session_id);
ErrorCode display_main_menu(const char *session_id);
ErrorCode display_student_menu(const char *session_id);
ErrorCode display_officer_menu(const char *session_id);
ErrorCode display_payment_menu(const char *session_id);
ErrorCode display_log_menu(const char *session_id);
void display_help(void);
```

### Cleanup
```c
void ui_cleanup(void);
```