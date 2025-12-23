#include "ui.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

// UI utility functions
void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void print_header(const char* title) {
    print_separator();
    print_centered_text(title);
    print_separator();
}

void print_menu(const char* title, const char* options[], int num_options) {
    print_header(title);
    
    for (int i = 0; i < num_options; i++) {
        printf("  %s\n", options[i]);
    }
    
    print_separator();
}

void print_separator() {
    printf(" =================================================================================\n");
}

void print_centered_text(const char* text) {
    const int width = 80;  // Assuming 80 character width
    int len = strlen(text);
    int padding = (width - len) / 2;
    
    if (padding > 0) {
        printf("%*s%s\n", padding, "", text);
    } else {
        printf("%s\n", text);
    }
}

void print_error(const char* message) {
    printf("\n*** ERROR: %s ***\n", message);
}

void print_success(const char* message) {
    printf("\n*** SUCCESS: %s ***\n", message);
}

void print_info(const char* message) {
    printf("\nINFO: %s\n", message);
}

// Input utility functions
int get_user_choice(int min, int max) {
    int choice;
    char input[10];
    
    while (1) {
        printf("\nEnter your choice (%d-%d): ", min, max);
        if (fgets(input, sizeof(input), stdin) != NULL) {
            if (sscanf(input, "%d", &choice) == 1) {
                if (choice >= min && choice <= max) {
                    return choice;
                }
            }
        }
        printf("Invalid input. Please enter a number between %d and %d.\n", min, max);
    }
}

char get_char_input() {
    char ch;
    #ifdef _WIN32
        ch = _getch();
    #else
        struct termios old_termios, new_termios;
        tcgetattr(STDIN_FILENO, &old_termios);
        new_termios = old_termios;
        new_termios.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
        
        ch = getchar();
        
        tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
    #endif
    return ch;
}

float get_float_input(const char* prompt) {
    float value;
    char input[50];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) != NULL) {
            if (sscanf(input, "%f", &value) == 1 && value >= 0) {
                return value;
            }
        }
        printf("Invalid input. Please enter a valid positive number.\n");
    }
}

int get_int_input(const char* prompt) {
    int value;
    char input[20];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) != NULL) {
            if (sscanf(input, "%d", &value) == 1) {
                return value;
            }
        }
        printf("Invalid input. Please enter a valid integer.\n");
    }
}

void get_string_input(char* buffer, size_t buffer_size, const char* prompt) {
    printf("%s", prompt);
    if (fgets(buffer, buffer_size, stdin) != NULL) {
        // Remove newline character if present
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
    }
}

// Display functions for different entities
void display_student(const Student* student) {
    if (!student) return;
    
    printf("  Student ID: %s\n", student->studentID);
    printf("  Name: %s\n", student->fullName);
    printf("  Email: %s\n", student->email);
    printf("  Phone: %s\n", student->phone);
    printf("  Registration Date: ");
    char date_str[20];
    format_date(student->registrationDate, date_str, sizeof(date_str));
    printf("%s\n", date_str);
    printf("  Status: %s\n", student->status);
}

void display_officer(const Officer* officer) {
    if (!officer) return;
    
    printf("  Officer ID: %s\n", officer->officerID);
    printf("  Name: %s\n", officer->name);
    printf("  Position: %s\n", officer->position);
    printf("  Access Level: %d\n", officer->accessLevel);
    printf("  Status: %s\n", officer->status);
    printf("  Last Login: ");
    char date_str[20];
    format_date(officer->lastLogin, date_str, sizeof(date_str));
    printf("%s\n", date_str);
}

void display_payment(const Payment* payment) {
    if (!payment) return;
    
    printf("  Payment ID: %s\n", payment->paymentID);
    printf("  Student ID: %s\n", payment->studentID);
    printf("  Amount: ");
    char amount_str[20];
    format_currency(payment->amount, amount_str, sizeof(amount_str));
    printf("%s\n", amount_str);
    printf("  Purpose: %s\n", payment->purpose);
    printf("  Date: ");
    char date_str[20];
    format_date(payment->paymentDate, date_str, sizeof(date_str));
    printf("%s\n", date_str);
    printf("  Status: %s\n", payment->status);
    printf("  Payment Method: %s\n", payment->paymentMethod);
}

void display_log_entry(const LogEntry* log) {
    if (!log) return;
    
    printf("  Log ID: %s\n", log->logID);
    printf("  User ID: %s\n", log->userID);
    printf("  Action: %s\n", log->action);
    printf("  Module: %s\n", log->module);
    printf("  Time: ");
    char time_str[20];
    format_date(log->timestamp, time_str, sizeof(time_str));
    printf("%s\n", time_str);
    printf("  Details: %s\n", log->details);
}

// List display functions
void display_student_list(const Student* students, int count) {
    if (!students || count <= 0) {
        printf("  No students found.\n");
        return;
    }
    
    printf("  %-15s %-30s %-20s %-12s %-10s\n", "ID", "Name", "Email", "Phone", "Status");
    print_separator();
    
    for (int i = 0; i < count; i++) {
        printf("  %-15s %-30s %-20s %-12s %-10s\n", 
               students[i].studentID, 
               students[i].fullName, 
               students[i].email, 
               students[i].phone, 
               students[i].status);
    }
}

void display_officer_list(const Officer* officers, int count) {
    if (!officers || count <= 0) {
        printf("  No officers found.\n");
        return;
    }
    
    printf("  %-15s %-25s %-20s %-12s %-10s\n", "ID", "Name", "Position", "Access Level", "Status");
    print_separator();
    
    for (int i = 0; i < count; i++) {
        printf("  %-15s %-25s %-20s %-12s %-10s\n", 
               officers[i].officerID, 
               officers[i].name, 
               officers[i].position, 
               officers[i].accessLevel, 
               officers[i].status);
    }
}

void display_payment_list(const Payment* payments, int count) {
    if (!payments || count <= 0) {
        printf("  No payments found.\n");
        return;
    }
    
    printf("  %-15s %-15s %-10s %-30s %-12s %-10s\n", "ID", "Student ID", "Amount", "Purpose", "Date", "Status");
    print_separator();
    
    for (int i = 0; i < count; i++) {
        char amount_str[20];
        format_currency(payments[i].amount, amount_str, sizeof(amount_str));
        
        char date_str[12];
        format_date(payments[i].paymentDate, date_str, sizeof(date_str));
        
        printf("  %-15s %-15s %-10s %-30s %-12s %-10s\n", 
               payments[i].paymentID, 
               payments[i].studentID, 
               amount_str, 
               payments[i].purpose, 
               date_str, 
               payments[i].status);
    }
}

void display_log_list(const LogEntry* logs, int count) {
    if (!logs || count <= 0) {
        printf("  No logs found.\n");
        return;
    }
    
    printf("  %-15s %-15s %-15s %-15s %-20s\n", "ID", "User ID", "Action", "Module", "Time");
    print_separator();
    
    for (int i = 0; i < count; i++) {
        char time_str[20];
        format_date(logs[i].timestamp, time_str, sizeof(time_str));
        
        printf("  %-15s %-15s %-15s %-15s %-20s\n", 
               logs[i].logID, 
               logs[i].userID, 
               logs[i].action, 
               logs[i].module, 
               time_str);
    }
}

// Formatting functions
void format_currency(float amount, char* buffer, size_t buffer_size) {
    if (!buffer || buffer_size == 0) return;
    
    snprintf(buffer, buffer_size, "%.2f", amount);
}

void format_date(time_t timestamp, char* buffer, size_t buffer_size) {
    if (!buffer || buffer_size == 0) return;
    
    struct tm* timeinfo = localtime(&timestamp);
    strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", timeinfo);
}