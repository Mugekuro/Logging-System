#ifndef FILE_IO_H
#define FILE_IO_H

#include "data_structures.h"
#include <stdio.h>

// File I/O functions for Students
ErrorCode save_student(const Student* student);
ErrorCode load_student(const char* student_id, Student* student);
ErrorCode update_student(const Student* student);
ErrorCode delete_student(const char* student_id);
ErrorCode list_all_students(Student** students, int* count);

// File I/O functions for Officers
ErrorCode save_officer(const Officer* officer);
ErrorCode load_officer(const char* officer_id, Officer* officer);
ErrorCode update_officer(const Officer* officer);
ErrorCode delete_officer(const char* officer_id);
ErrorCode list_all_officers(Officer** officers, int* count);

// File I/O functions for Payments
ErrorCode save_payment(const Payment* payment);
ErrorCode load_payment(const char* payment_id, Payment* payment);
ErrorCode update_payment(const Payment* payment);
ErrorCode delete_payment(const char* payment_id);
ErrorCode list_all_payments(Payment** payments, int* count);

// File I/O functions for Logs
ErrorCode save_log(const LogEntry* log);
ErrorCode load_logs_by_date(const char* date, LogEntry** logs, int* count);
ErrorCode load_logs_by_user(const char* user_id, LogEntry** logs, int* count);
ErrorCode load_all_logs(LogEntry** logs, int* count);

// Generic file utility functions
ErrorCode create_backup(const char* filename, const char* backup_dir);
ErrorCode restore_from_backup(const char* backup_file, const char* target_file);
long get_file_size(const char* filename);
int file_exists(const char* filename);
ErrorCode ensure_directory_exists(const char* dir_path);

#endif // FILE_IO_H