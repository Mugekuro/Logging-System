#include "file_io.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define file paths
#define STUDENTS_FILE "data/students.dat"
#define OFFICERS_FILE "data/officers.dat"
#define PAYMENTS_FILE "data/payments.dat"
#define LOGS_FILE "data/logs.dat"

// Helper function to get file size
long get_file_size(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        return -1;
    }
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    
    return size;
}

// Helper function to check if file exists
int file_exists(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

// Helper function to ensure directory exists
ErrorCode ensure_directory_exists(const char* dir_path) {
    // On Windows, we'd use CreateDirectory, but for portability we'll use system command
    // This is a simplified implementation - in production, use platform-specific APIs
    char command[512];
    snprintf(command, sizeof(command), "mkdir \"%s\" 2>nul", dir_path);
    
    int result = system(command);
    if (result == 0 || result == 1) {  // mkdir returns 1 if directory already exists
        return SUCCESS;
    }
    
    return ERROR_FILE_OPERATION;
}

// File I/O functions for Students
ErrorCode save_student(const Student* student) {
    if (!student) {
        return ERROR_INVALID_INPUT;
    }
    
    // Ensure data directory exists
    ErrorCode dir_result = ensure_directory_exists("data");
    if (dir_result != SUCCESS) {
        return dir_result;
    }
    
    FILE* file = fopen(STUDENTS_FILE, "ab");
    if (!file) {
        log_error(ERROR_FILE_OPERATION, "save_student", "Could not open students file for writing");
        return ERROR_FILE_OPERATION;
    }
    
    if (fwrite(student, sizeof(Student), 1, file) != 1) {
        fclose(file);
        log_error(ERROR_FILE_OPERATION, "save_student", "Could not write student data");
        return ERROR_FILE_OPERATION;
    }
    
    fclose(file);
    return SUCCESS;
}

ErrorCode load_student(const char* student_id, Student* student) {
    if (!student_id || !student) {
        return ERROR_INVALID_INPUT;
    }
    
    if (!file_exists(STUDENTS_FILE)) {
        return ERROR_DATA_NOT_FOUND;
    }
    
    FILE* file = fopen(STUDENTS_FILE, "rb");
    if (!file) {
        log_error(ERROR_FILE_OPERATION, "load_student", "Could not open students file for reading");
        return ERROR_FILE_OPERATION;
    }
    
    Student temp_student;
    while (fread(&temp_student, sizeof(Student), 1, file) == 1) {
        if (strcmp(temp_student.studentID, student_id) == 0) {
            *student = temp_student;
            fclose(file);
            return SUCCESS;
        }
    }
    
    fclose(file);
    return ERROR_DATA_NOT_FOUND;
}

ErrorCode update_student(const Student* student) {
    if (!student) {
        return ERROR_INVALID_INPUT;
    }
    
    if (!file_exists(STUDENTS_FILE)) {
        return ERROR_DATA_NOT_FOUND;
    }
    
    // Read all students
    FILE* file = fopen(STUDENTS_FILE, "rb");
    if (!file) {
        return ERROR_FILE_OPERATION;
    }
    
    Student* all_students = NULL;
    long file_size = get_file_size(STUDENTS_FILE);
    if (file_size < 0) {
        fclose(file);
        return ERROR_FILE_OPERATION;
    }
    
    int student_count = file_size / sizeof(Student);
    all_students = (Student*)safe_malloc(student_count * sizeof(Student));
    if (!all_students) {
        fclose(file);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    int read_count = fread(all_students, sizeof(Student), student_count, file);
    fclose(file);
    
    if (read_count != student_count) {
        safe_free((void**)&all_students);
        return ERROR_FILE_OPERATION;
    }
    
    // Find and update the student
    int found = 0;
    for (int i = 0; i < student_count; i++) {
        if (strcmp(all_students[i].studentID, student->studentID) == 0) {
            all_students[i] = *student;
            found = 1;
            break;
        }
    }
    
    if (!found) {
        safe_free((void**)&all_students);
        return ERROR_DATA_NOT_FOUND;
    }
    
    // Write all students back to file
    file = fopen(STUDENTS_FILE, "wb");
    if (!file) {
        safe_free((void**)&all_students);
        return ERROR_FILE_OPERATION;
    }
    
    if (fwrite(all_students, sizeof(Student), student_count, file) != student_count) {
        fclose(file);
        safe_free((void**)&all_students);
        return ERROR_FILE_OPERATION;
    }
    
    fclose(file);
    safe_free((void**)&all_students);
    return SUCCESS;
}

ErrorCode delete_student(const char* student_id) {
    if (!student_id) {
        return ERROR_INVALID_INPUT;
    }
    
    if (!file_exists(STUDENTS_FILE)) {
        return ERROR_DATA_NOT_FOUND;
    }
    
    // Read all students
    FILE* file = fopen(STUDENTS_FILE, "rb");
    if (!file) {
        return ERROR_FILE_OPERATION;
    }
    
    Student* all_students = NULL;
    long file_size = get_file_size(STUDENTS_FILE);
    if (file_size < 0) {
        fclose(file);
        return ERROR_FILE_OPERATION;
    }
    
    int student_count = file_size / sizeof(Student);
    all_students = (Student*)safe_malloc(student_count * sizeof(Student));
    if (!all_students) {
        fclose(file);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    int read_count = fread(all_students, sizeof(Student), student_count, file);
    fclose(file);
    
    if (read_count != student_count) {
        safe_free((void**)&all_students);
        return ERROR_FILE_OPERATION;
    }
    
    // Find and remove the student
    int found = 0;
    int new_count = 0;
    Student* new_students = (Student*)safe_malloc(student_count * sizeof(Student));
    if (!new_students) {
        safe_free((void**)&all_students);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    for (int i = 0; i < student_count; i++) {
        if (strcmp(all_students[i].studentID, student_id) != 0) {
            new_students[new_count] = all_students[i];
            new_count++;
        } else {
            found = 1;
        }
    }
    
    safe_free((void**)&all_students);
    
    if (!found) {
        safe_free((void**)&new_students);
        return ERROR_DATA_NOT_FOUND;
    }
    
    // Write remaining students back to file
    file = fopen(STUDENTS_FILE, "wb");
    if (!file) {
        safe_free((void**)&new_students);
        return ERROR_FILE_OPERATION;
    }
    
    if (fwrite(new_students, sizeof(Student), new_count, file) != new_count) {
        fclose(file);
        safe_free((void**)&new_students);
        return ERROR_FILE_OPERATION;
    }
    
    fclose(file);
    safe_free((void**)&new_students);
    return SUCCESS;
}

ErrorCode list_all_students(Student** students, int* count) {
    if (!students || !count) {
        return ERROR_INVALID_INPUT;
    }
    
    if (!file_exists(STUDENTS_FILE)) {
        *students = NULL;
        *count = 0;
        return SUCCESS;
    }
    
    FILE* file = fopen(STUDENTS_FILE, "rb");
    if (!file) {
        return ERROR_FILE_OPERATION;
    }
    
    long file_size = get_file_size(STUDENTS_FILE);
    if (file_size < 0) {
        fclose(file);
        return ERROR_FILE_OPERATION;
    }
    
    int student_count = file_size / sizeof(Student);
    if (student_count == 0) {
        *students = NULL;
        *count = 0;
        fclose(file);
        return SUCCESS;
    }
    
    Student* all_students = (Student*)safe_malloc(student_count * sizeof(Student));
    if (!all_students) {
        fclose(file);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    int read_count = fread(all_students, sizeof(Student), student_count, file);
    fclose(file);
    
    if (read_count != student_count) {
        safe_free((void**)&all_students);
        return ERROR_FILE_OPERATION;
    }
    
    *students = all_students;
    *count = student_count;
    return SUCCESS;
}

// File I/O functions for Officers
ErrorCode save_officer(const Officer* officer) {
    if (!officer) {
        return ERROR_INVALID_INPUT;
    }
    
    // Ensure data directory exists
    ErrorCode dir_result = ensure_directory_exists("data");
    if (dir_result != SUCCESS) {
        return dir_result;
    }
    
    FILE* file = fopen(OFFICERS_FILE, "ab");
    if (!file) {
        log_error(ERROR_FILE_OPERATION, "save_officer", "Could not open officers file for writing");
        return ERROR_FILE_OPERATION;
    }
    
    if (fwrite(officer, sizeof(Officer), 1, file) != 1) {
        fclose(file);
        log_error(ERROR_FILE_OPERATION, "save_officer", "Could not write officer data");
        return ERROR_FILE_OPERATION;
    }
    
    fclose(file);
    return SUCCESS;
}

ErrorCode load_officer(const char* officer_id, Officer* officer) {
    if (!officer_id || !officer) {
        return ERROR_INVALID_INPUT;
    }
    
    if (!file_exists(OFFICERS_FILE)) {
        return ERROR_DATA_NOT_FOUND;
    }
    
    FILE* file = fopen(OFFICERS_FILE, "rb");
    if (!file) {
        log_error(ERROR_FILE_OPERATION, "load_officer", "Could not open officers file for reading");
        return ERROR_FILE_OPERATION;
    }
    
    Officer temp_officer;
    while (fread(&temp_officer, sizeof(Officer), 1, file) == 1) {
        if (strcmp(temp_officer.officerID, officer_id) == 0) {
            *officer = temp_officer;
            fclose(file);
            return SUCCESS;
        }
    }
    
    fclose(file);
    return ERROR_DATA_NOT_FOUND;
}

ErrorCode update_officer(const Officer* officer) {
    if (!officer) {
        return ERROR_INVALID_INPUT;
    }
    
    if (!file_exists(OFFICERS_FILE)) {
        return ERROR_DATA_NOT_FOUND;
    }
    
    // Read all officers
    FILE* file = fopen(OFFICERS_FILE, "rb");
    if (!file) {
        return ERROR_FILE_OPERATION;
    }
    
    Officer* all_officers = NULL;
    long file_size = get_file_size(OFFICERS_FILE);
    if (file_size < 0) {
        fclose(file);
        return ERROR_FILE_OPERATION;
    }
    
    int officer_count = file_size / sizeof(Officer);
    all_officers = (Officer*)safe_malloc(officer_count * sizeof(Officer));
    if (!all_officers) {
        fclose(file);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    int read_count = fread(all_officers, sizeof(Officer), officer_count, file);
    fclose(file);
    
    if (read_count != officer_count) {
        safe_free((void**)&all_officers);
        return ERROR_FILE_OPERATION;
    }
    
    // Find and update the officer
    int found = 0;
    for (int i = 0; i < officer_count; i++) {
        if (strcmp(all_officers[i].officerID, officer->officerID) == 0) {
            all_officers[i] = *officer;
            found = 1;
            break;
        }
    }
    
    if (!found) {
        safe_free((void**)&all_officers);
        return ERROR_DATA_NOT_FOUND;
    }
    
    // Write all officers back to file
    file = fopen(OFFICERS_FILE, "wb");
    if (!file) {
        safe_free((void**)&all_officers);
        return ERROR_FILE_OPERATION;
    }
    
    if (fwrite(all_officers, sizeof(Officer), officer_count, file) != officer_count) {
        fclose(file);
        safe_free((void**)&all_officers);
        return ERROR_FILE_OPERATION;
    }
    
    fclose(file);
    safe_free((void**)&all_officers);
    return SUCCESS;
}

ErrorCode delete_officer(const char* officer_id) {
    if (!officer_id) {
        return ERROR_INVALID_INPUT;
    }
    
    if (!file_exists(OFFICERS_FILE)) {
        return ERROR_DATA_NOT_FOUND;
    }
    
    // Read all officers
    FILE* file = fopen(OFFICERS_FILE, "rb");
    if (!file) {
        return ERROR_FILE_OPERATION;
    }
    
    Officer* all_officers = NULL;
    long file_size = get_file_size(OFFICERS_FILE);
    if (file_size < 0) {
        fclose(file);
        return ERROR_FILE_OPERATION;
    }
    
    int officer_count = file_size / sizeof(Officer);
    all_officers = (Officer*)safe_malloc(officer_count * sizeof(Officer));
    if (!all_officers) {
        fclose(file);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    int read_count = fread(all_officers, sizeof(Officer), officer_count, file);
    fclose(file);
    
    if (read_count != officer_count) {
        safe_free((void**)&all_officers);
        return ERROR_FILE_OPERATION;
    }
    
    // Find and remove the officer
    int found = 0;
    int new_count = 0;
    Officer* new_officers = (Officer*)safe_malloc(officer_count * sizeof(Officer));
    if (!new_officers) {
        safe_free((void**)&all_officers);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    for (int i = 0; i < officer_count; i++) {
        if (strcmp(all_officers[i].officerID, officer_id) != 0) {
            new_officers[new_count] = all_officers[i];
            new_count++;
        } else {
            found = 1;
        }
    }
    
    safe_free((void**)&all_officers);
    
    if (!found) {
        safe_free((void**)&new_officers);
        return ERROR_DATA_NOT_FOUND;
    }
    
    // Write remaining officers back to file
    file = fopen(OFFICERS_FILE, "wb");
    if (!file) {
        safe_free((void**)&new_officers);
        return ERROR_FILE_OPERATION;
    }
    
    if (fwrite(new_officers, sizeof(Officer), new_count, file) != new_count) {
        fclose(file);
        safe_free((void**)&new_officers);
        return ERROR_FILE_OPERATION;
    }
    
    fclose(file);
    safe_free((void**)&new_officers);
    return SUCCESS;
}

ErrorCode list_all_officers(Officer** officers, int* count) {
    if (!officers || !count) {
        return ERROR_INVALID_INPUT;
    }
    
    if (!file_exists(OFFICERS_FILE)) {
        *officers = NULL;
        *count = 0;
        return SUCCESS;
    }
    
    FILE* file = fopen(OFFICERS_FILE, "rb");
    if (!file) {
        return ERROR_FILE_OPERATION;
    }
    
    long file_size = get_file_size(OFFICERS_FILE);
    if (file_size < 0) {
        fclose(file);
        return ERROR_FILE_OPERATION;
    }
    
    int officer_count = file_size / sizeof(Officer);
    if (officer_count == 0) {
        *officers = NULL;
        *count = 0;
        fclose(file);
        return SUCCESS;
    }
    
    Officer* all_officers = (Officer*)safe_malloc(officer_count * sizeof(Officer));
    if (!all_officers) {
        fclose(file);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    int read_count = fread(all_officers, sizeof(Officer), officer_count, file);
    fclose(file);
    
    if (read_count != officer_count) {
        safe_free((void**)&all_officers);
        return ERROR_FILE_OPERATION;
    }
    
    *officers = all_officers;
    *count = officer_count;
    return SUCCESS;
}

// File I/O functions for Payments
ErrorCode save_payment(const Payment* payment) {
    if (!payment) {
        return ERROR_INVALID_INPUT;
    }
    
    // Ensure data directory exists
    ErrorCode dir_result = ensure_directory_exists("data");
    if (dir_result != SUCCESS) {
        return dir_result;
    }
    
    FILE* file = fopen(PAYMENTS_FILE, "ab");
    if (!file) {
        log_error(ERROR_FILE_OPERATION, "save_payment", "Could not open payments file for writing");
        return ERROR_FILE_OPERATION;
    }
    
    if (fwrite(payment, sizeof(Payment), 1, file) != 1) {
        fclose(file);
        log_error(ERROR_FILE_OPERATION, "save_payment", "Could not write payment data");
        return ERROR_FILE_OPERATION;
    }
    
    fclose(file);
    return SUCCESS;
}

ErrorCode load_payment(const char* payment_id, Payment* payment) {
    if (!payment_id || !payment) {
        return ERROR_INVALID_INPUT;
    }
    
    if (!file_exists(PAYMENTS_FILE)) {
        return ERROR_DATA_NOT_FOUND;
    }
    
    FILE* file = fopen(PAYMENTS_FILE, "rb");
    if (!file) {
        log_error(ERROR_FILE_OPERATION, "load_payment", "Could not open payments file for reading");
        return ERROR_FILE_OPERATION;
    }
    
    Payment temp_payment;
    while (fread(&temp_payment, sizeof(Payment), 1, file) == 1) {
        if (strcmp(temp_payment.paymentID, payment_id) == 0) {
            *payment = temp_payment;
            fclose(file);
            return SUCCESS;
        }
    }
    
    fclose(file);
    return ERROR_DATA_NOT_FOUND;
}

ErrorCode update_payment(const Payment* payment) {
    if (!payment) {
        return ERROR_INVALID_INPUT;
    }
    
    if (!file_exists(PAYMENTS_FILE)) {
        return ERROR_DATA_NOT_FOUND;
    }
    
    // Read all payments
    FILE* file = fopen(PAYMENTS_FILE, "rb");
    if (!file) {
        return ERROR_FILE_OPERATION;
    }
    
    Payment* all_payments = NULL;
    long file_size = get_file_size(PAYMENTS_FILE);
    if (file_size < 0) {
        fclose(file);
        return ERROR_FILE_OPERATION;
    }
    
    int payment_count = file_size / sizeof(Payment);
    all_payments = (Payment*)safe_malloc(payment_count * sizeof(Payment));
    if (!all_payments) {
        fclose(file);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    int read_count = fread(all_payments, sizeof(Payment), payment_count, file);
    fclose(file);
    
    if (read_count != payment_count) {
        safe_free((void**)&all_payments);
        return ERROR_FILE_OPERATION;
    }
    
    // Find and update the payment
    int found = 0;
    for (int i = 0; i < payment_count; i++) {
        if (strcmp(all_payments[i].paymentID, payment->paymentID) == 0) {
            all_payments[i] = *payment;
            found = 1;
            break;
        }
    }
    
    if (!found) {
        safe_free((void**)&all_payments);
        return ERROR_DATA_NOT_FOUND;
    }
    
    // Write all payments back to file
    file = fopen(PAYMENTS_FILE, "wb");
    if (!file) {
        safe_free((void**)&all_payments);
        return ERROR_FILE_OPERATION;
    }
    
    if (fwrite(all_payments, sizeof(Payment), payment_count, file) != payment_count) {
        fclose(file);
        safe_free((void**)&all_payments);
        return ERROR_FILE_OPERATION;
    }
    
    fclose(file);
    safe_free((void**)&all_payments);
    return SUCCESS;
}

ErrorCode delete_payment(const char* payment_id) {
    if (!payment_id) {
        return ERROR_INVALID_INPUT;
    }
    
    if (!file_exists(PAYMENTS_FILE)) {
        return ERROR_DATA_NOT_FOUND;
    }
    
    // Read all payments
    FILE* file = fopen(PAYMENTS_FILE, "rb");
    if (!file) {
        return ERROR_FILE_OPERATION;
    }
    
    Payment* all_payments = NULL;
    long file_size = get_file_size(PAYMENTS_FILE);
    if (file_size < 0) {
        fclose(file);
        return ERROR_FILE_OPERATION;
    }
    
    int payment_count = file_size / sizeof(Payment);
    all_payments = (Payment*)safe_malloc(payment_count * sizeof(Payment));
    if (!all_payments) {
        fclose(file);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    int read_count = fread(all_payments, sizeof(Payment), payment_count, file);
    fclose(file);
    
    if (read_count != payment_count) {
        safe_free((void**)&all_payments);
        return ERROR_FILE_OPERATION;
    }
    
    // Find and remove the payment
    int found = 0;
    int new_count = 0;
    Payment* new_payments = (Payment*)safe_malloc(payment_count * sizeof(Payment));
    if (!new_payments) {
        safe_free((void**)&all_payments);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    for (int i = 0; i < payment_count; i++) {
        if (strcmp(all_payments[i].paymentID, payment_id) != 0) {
            new_payments[new_count] = all_payments[i];
            new_count++;
        } else {
            found = 1;
        }
    }
    
    safe_free((void**)&all_payments);
    
    if (!found) {
        safe_free((void**)&new_payments);
        return ERROR_DATA_NOT_FOUND;
    }
    
    // Write remaining payments back to file
    file = fopen(PAYMENTS_FILE, "wb");
    if (!file) {
        safe_free((void**)&new_payments);
        return ERROR_FILE_OPERATION;
    }
    
    if (fwrite(new_payments, sizeof(Payment), new_count, file) != new_count) {
        fclose(file);
        safe_free((void**)&new_payments);
        return ERROR_FILE_OPERATION;
    }
    
    fclose(file);
    safe_free((void**)&new_payments);
    return SUCCESS;
}

ErrorCode list_all_payments(Payment** payments, int* count) {
    if (!payments || !count) {
        return ERROR_INVALID_INPUT;
    }
    
    if (!file_exists(PAYMENTS_FILE)) {
        *payments = NULL;
        *count = 0;
        return SUCCESS;
    }
    
    FILE* file = fopen(PAYMENTS_FILE, "rb");
    if (!file) {
        return ERROR_FILE_OPERATION;
    }
    
    long file_size = get_file_size(PAYMENTS_FILE);
    if (file_size < 0) {
        fclose(file);
        return ERROR_FILE_OPERATION;
    }
    
    int payment_count = file_size / sizeof(Payment);
    if (payment_count == 0) {
        *payments = NULL;
        *count = 0;
        fclose(file);
        return SUCCESS;
    }
    
    Payment* all_payments = (Payment*)safe_malloc(payment_count * sizeof(Payment));
    if (!all_payments) {
        fclose(file);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    int read_count = fread(all_payments, sizeof(Payment), payment_count, file);
    fclose(file);
    
    if (read_count != payment_count) {
        safe_free((void**)&all_payments);
        return ERROR_FILE_OPERATION;
    }
    
    *payments = all_payments;
    *count = payment_count;
    return SUCCESS;
}

// File I/O functions for Logs
ErrorCode save_log(const LogEntry* log) {
    if (!log) {
        return ERROR_INVALID_INPUT;
    }
    
    // Ensure data directory exists
    ErrorCode dir_result = ensure_directory_exists("data");
    if (dir_result != SUCCESS) {
        return dir_result;
    }
    
    FILE* file = fopen(LOGS_FILE, "ab");
    if (!file) {
        log_error(ERROR_FILE_OPERATION, "save_log", "Could not open logs file for writing");
        return ERROR_FILE_OPERATION;
    }
    
    if (fwrite(log, sizeof(LogEntry), 1, file) != 1) {
        fclose(file);
        log_error(ERROR_FILE_OPERATION, "save_log", "Could not write log data");
        return ERROR_FILE_OPERATION;
    }
    
    fclose(file);
    return SUCCESS;
}

ErrorCode load_logs_by_date(const char* date, LogEntry** logs, int* count) {
    if (!date || !logs || !count) {
        return ERROR_INVALID_INPUT;
    }
    
    if (!file_exists(LOGS_FILE)) {
        *logs = NULL;
        *count = 0;
        return SUCCESS;
    }
    
    FILE* file = fopen(LOGS_FILE, "rb");
    if (!file) {
        return ERROR_FILE_OPERATION;
    }
    
    // First, count matching logs
    LogEntry temp_log;
    int matching_count = 0;
    while (fread(&temp_log, sizeof(LogEntry), 1, file) == 1) {
        char date_str[11];
        strftime(date_str, sizeof(date_str), "%Y-%m-%d", localtime(&temp_log.timestamp));
        if (strcmp(date_str, date) == 0) {
            matching_count++;
        }
    }
    
    if (matching_count == 0) {
        *logs = NULL;
        *count = 0;
        fclose(file);
        return SUCCESS;
    }
    
    // Allocate memory for logs
    LogEntry* matching_logs = (LogEntry*)safe_malloc(matching_count * sizeof(LogEntry));
    if (!matching_logs) {
        fclose(file);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    // Read matching logs
    rewind(file);
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

ErrorCode load_logs_by_user(const char* user_id, LogEntry** logs, int* count) {
    if (!user_id || !logs || !count) {
        return ERROR_INVALID_INPUT;
    }
    
    if (!file_exists(LOGS_FILE)) {
        *logs = NULL;
        *count = 0;
        return SUCCESS;
    }
    
    FILE* file = fopen(LOGS_FILE, "rb");
    if (!file) {
        return ERROR_FILE_OPERATION;
    }
    
    // First, count matching logs
    LogEntry temp_log;
    int matching_count = 0;
    while (fread(&temp_log, sizeof(LogEntry), 1, file) == 1) {
        if (strcmp(temp_log.userID, user_id) == 0) {
            matching_count++;
        }
    }
    
    if (matching_count == 0) {
        *logs = NULL;
        *count = 0;
        fclose(file);
        return SUCCESS;
    }
    
    // Allocate memory for logs
    LogEntry* matching_logs = (LogEntry*)safe_malloc(matching_count * sizeof(LogEntry));
    if (!matching_logs) {
        fclose(file);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    // Read matching logs
    rewind(file);
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

ErrorCode load_all_logs(LogEntry** logs, int* count) {
    if (!logs || !count) {
        return ERROR_INVALID_INPUT;
    }
    
    if (!file_exists(LOGS_FILE)) {
        *logs = NULL;
        *count = 0;
        return SUCCESS;
    }
    
    FILE* file = fopen(LOGS_FILE, "rb");
    if (!file) {
        return ERROR_FILE_OPERATION;
    }
    
    long file_size = get_file_size(LOGS_FILE);
    if (file_size < 0) {
        fclose(file);
        return ERROR_FILE_OPERATION;
    }
    
    int log_count = file_size / sizeof(LogEntry);
    if (log_count == 0) {
        *logs = NULL;
        *count = 0;
        fclose(file);
        return SUCCESS;
    }
    
    LogEntry* all_logs = (LogEntry*)safe_malloc(log_count * sizeof(LogEntry));
    if (!all_logs) {
        fclose(file);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    int read_count = fread(all_logs, sizeof(LogEntry), log_count, file);
    fclose(file);
    
    if (read_count != log_count) {
        safe_free((void**)&all_logs);
        return ERROR_FILE_OPERATION;
    }
    
    *logs = all_logs;
    *count = log_count;
    return SUCCESS;
}

// Generic file utility functions
ErrorCode create_backup(const char* filename, const char* backup_dir) {
    if (!filename || !backup_dir) {
        return ERROR_INVALID_INPUT;
    }
    
    // Ensure backup directory exists
    ErrorCode dir_result = ensure_directory_exists(backup_dir);
    if (dir_result != SUCCESS) {
        return dir_result;
    }
    
    // Create backup filename with timestamp
    time_t now;
    time(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", localtime(&now));
    
    char backup_path[512];
    snprintf(backup_path, sizeof(backup_path), "%s/%s_%s.bak", backup_dir, filename, timestamp);
    
    // Copy file
    FILE* source = fopen(filename, "rb");
    if (!source) {
        return ERROR_FILE_OPERATION;
    }
    
    FILE* dest = fopen(backup_path, "wb");
    if (!dest) {
        fclose(source);
        return ERROR_FILE_OPERATION;
    }
    
    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        if (fwrite(buffer, 1, bytes, dest) != bytes) {
            fclose(source);
            fclose(dest);
            remove(backup_path);  // Remove partial backup
            return ERROR_FILE_OPERATION;
        }
    }
    
    fclose(source);
    fclose(dest);
    
    return SUCCESS;
}

ErrorCode restore_from_backup(const char* backup_file, const char* target_file) {
    if (!backup_file || !target_file) {
        return ERROR_INVALID_INPUT;
    }
    
    // Copy backup file to target
    FILE* source = fopen(backup_file, "rb");
    if (!source) {
        return ERROR_FILE_OPERATION;
    }
    
    FILE* dest = fopen(target_file, "wb");
    if (!dest) {
        fclose(source);
        return ERROR_FILE_OPERATION;
    }
    
    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        if (fwrite(buffer, 1, bytes, dest) != bytes) {
            fclose(source);
            fclose(dest);
            return ERROR_FILE_OPERATION;
        }
    }
    
    fclose(source);
    fclose(dest);
    
    return SUCCESS;
}