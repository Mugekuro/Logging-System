#ifndef UI_H
#define UI_H

#include "data_structures.h"
#include <stdio.h>

// UI utility functions
void clear_screen();
void print_header(const char* title);
void print_menu(const char* title, const char* options[], int num_options);
void print_separator();
void print_centered_text(const char* text);
void print_error(const char* message);
void print_success(const char* message);
void print_info(const char* message);

// Input utility functions
int get_user_choice(int min, int max);
char get_char_input();
float get_float_input(const char* prompt);
int get_int_input(const char* prompt);
void get_string_input(char* buffer, size_t buffer_size, const char* prompt);

// Display functions for different entities
void display_student(const Student* student);
void display_officer(const Officer* officer);
void display_payment(const Payment* payment);
void display_log_entry(const LogEntry* log);

// List display functions
void display_student_list(const Student* students, int count);
void display_officer_list(const Officer* officers, int count);
void display_payment_list(const Payment* payments, int count);
void display_log_list(const LogEntry* logs, int count);

// Formatting functions
void format_currency(float amount, char* buffer, size_t buffer_size);
void format_date(time_t timestamp, char* buffer, size_t buffer_size);

#endif // UI_H