#ifndef UTILS_H
#define UTILS_H

#include "data_structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Error handling functions
const char* get_error_message(ErrorCode code);
void log_error(ErrorCode code, const char* function, const char* message);

// Input validation functions
bool is_valid_email(const char* email);
bool is_valid_phone(const char* phone);
bool is_valid_name(const char* name);
bool is_valid_id(const char* id);
bool is_valid_amount(float amount);
bool is_valid_date(const char* date);  // Format: YYYY-MM-DD

// String utility functions
char* trim_whitespace(char* str);
int safe_strcpy(char* dest, const char* src, size_t dest_size);
int safe_strcat(char* dest, const char* src, size_t dest_size);

// Time utility functions
char* format_timestamp(time_t timestamp, char* buffer, size_t buf_size);
time_t parse_date_string(const char* date_str);  // Format: YYYY-MM-DD

// Memory utility functions
void* safe_malloc(size_t size);
void safe_free(void** ptr);

// Generate unique ID
int generate_unique_id(char* id_buffer, size_t buffer_size, const char* prefix);

// Password utility functions
int hash_password(const char* plain_password, char* hashed_password, size_t hash_size);
int verify_password(const char* plain_password, const char* hashed_password);

// Additional validation functions needed by main.c
bool validate_student_input(const Student *student);
bool validate_officer_input(const Officer *officer);
bool validate_payment_input(const Payment *payment);

// Date/time utility functions needed by main.c
void getCurrentDateTimeString(char* buffer, size_t buffer_size);
void getCurrentDateString(char* buffer, size_t buffer_size);
bool isValidDate(const char* date_str);

#endif // UTILS_H