#include "search_report.h"
#include "file_io.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Search students with multiple criteria
ErrorCode search_students(const StudentSearchCriteria* criteria, SearchResults* results) {
    if (!criteria || !results) {
        return ERROR_INVALID_INPUT;
    }
    
    // Get all students first
    Student all_students[MAX_STUDENTS];
    int total_students = 0;
    ErrorCode status = get_all_students(all_students, MAX_STUDENTS, &total_students);
    
    if (status != SUCCESS) {
        return status;
    }
    
    // Allocate memory for results
    results->students = malloc(sizeof(Student) * total_students);
    if (!results->students) {
        return ERROR_MEMORY_ALLOCATION;
    }
    
    results->student_count = 0;
    
    // Filter students based on criteria
    for (int i = 0; i < total_students; i++) {
        bool matches = true;
        
        // Check student ID
        if (strlen(criteria->student_id) > 0) {
            if (strstr(all_students[i].studentID, criteria->student_id) == NULL) {
                matches = false;
            }
        }
        
        // Check first name
        if (strlen(criteria->first_name) > 0) {
            if (strstr(all_students[i].firstName, criteria->first_name) == NULL) {
                matches = false;
            }
        }
        
        // Check last name
        if (strlen(criteria->last_name) > 0) {
            if (strstr(all_students[i].lastName, criteria->last_name) == NULL) {
                matches = false;
            }
        }
        
        // Check email
        if (strlen(criteria->email) > 0) {
            if (strstr(all_students[i].email, criteria->email) == NULL) {
                matches = false;
            }
        }
        
        // Check status
        if (strlen(criteria->status) > 0) {
            if (strcmp(all_students[i].status, criteria->status) != 0) {
                matches = false;
            }
        }
        
        // Check registration date range
        if (criteria->use_registration_date_range) {
            if (all_students[i].registrationDate < criteria->registration_date_from ||
                all_students[i].registrationDate > criteria->registration_date_to) {
                matches = false;
            }
        }
        
        if (matches) {
            results->students[results->student_count] = all_students[i];
            results->student_count++;
        }
    }
    
    return SUCCESS;
}

// Search officers with multiple criteria
ErrorCode search_officers(const OfficerSearchCriteria* criteria, SearchResults* results) {
    if (!criteria || !results) {
        return ERROR_INVALID_INPUT;
    }
    
    // Get all officers first
    Officer all_officers[MAX_OFFICERS];
    int total_officers = 0;
    ErrorCode status = get_all_officers(all_officers, MAX_OFFICERS, &total_officers);
    
    if (status != SUCCESS) {
        return status;
    }
    
    // Allocate memory for results
    results->officers = malloc(sizeof(Officer) * total_officers);
    if (!results->officers) {
        return ERROR_MEMORY_ALLOCATION;
    }
    
    results->officer_count = 0;
    
    // Filter officers based on criteria
    for (int i = 0; i < total_officers; i++) {
        bool matches = true;
        
        // Check officer ID
        if (strlen(criteria->officer_id) > 0) {
            if (strstr(all_officers[i].officerID, criteria->officer_id) == NULL) {
                matches = false;
            }
        }
        
        // Check name
        if (strlen(criteria->name) > 0) {
            if (strstr(all_officers[i].name, criteria->name) == NULL) {
                matches = false;
            }
        }
        
        // Check position
        if (strlen(criteria->position) > 0) {
            if (strstr(all_officers[i].position, criteria->position) == NULL) {
                matches = false;
            }
        }
        
        // Check access level range
        if (criteria->use_access_level_range) {
            if (all_officers[i].accessLevel < criteria->access_level_min ||
                all_officers[i].accessLevel > criteria->access_level_max) {
                matches = false;
            }
        }
        
        // Check last login range
        if (criteria->use_last_login_range) {
            if (all_officers[i].lastLogin < criteria->last_login_from ||
                all_officers[i].lastLogin > criteria->last_login_to) {
                matches = false;
            }
        }
        
        if (matches) {
            results->officers[results->officer_count] = all_officers[i];
            results->officer_count++;
        }
    }
    
    return SUCCESS;
}

// Search payments with multiple criteria
ErrorCode search_payments(const PaymentSearchCriteria* criteria, SearchResults* results) {
    if (!criteria || !results) {
        return ERROR_INVALID_INPUT;
    }
    
    // Get all payments first
    Payment all_payments[MAX_PAYMENTS];
    int total_payments = 0;
    ErrorCode status = get_all_payments(all_payments, MAX_PAYMENTS, &total_payments);
    
    if (status != SUCCESS) {
        return status;
    }
    
    // Allocate memory for results
    results->payments = malloc(sizeof(Payment) * total_payments);
    if (!results->payments) {
        return ERROR_MEMORY_ALLOCATION;
    }
    
    results->payment_count = 0;
    
    // Filter payments based on criteria
    for (int i = 0; i < total_payments; i++) {
        bool matches = true;
        
        // Check payment ID
        if (strlen(criteria->payment_id) > 0) {
            if (strstr(all_payments[i].paymentID, criteria->payment_id) == NULL) {
                matches = false;
            }
        }
        
        // Check student ID
        if (strlen(criteria->student_id) > 0) {
            if (strstr(all_payments[i].studentID, criteria->student_id) == NULL) {
                matches = false;
            }
        }
        
        // Check purpose
        if (strlen(criteria->purpose) > 0) {
            if (strstr(all_payments[i].purpose, criteria->purpose) == NULL) {
                matches = false;
            }
        }
        
        // Check amount range
        if (criteria->use_amount_range) {
            if (all_payments[i].amount < criteria->amount_min ||
                all_payments[i].amount > criteria->amount_max) {
                matches = false;
            }
        }
        
        // Check date range
        if (criteria->use_date_range) {
            if (all_payments[i].paymentDate < criteria->payment_date_from ||
                all_payments[i].paymentDate > criteria->payment_date_to) {
                matches = false;
            }
        }
        
        // Check payment method
        if (strlen(criteria->payment_method) > 0) {
            if (strcmp(all_payments[i].paymentMethod, criteria->payment_method) != 0) {
                matches = false;
            }
        }
        
        // Check status
        if (strlen(criteria->status) > 0) {
            if (strcmp(all_payments[i].status, criteria->status) != 0) {
                matches = false;
            }
        }
        
        if (matches) {
            results->payments[results->payment_count] = all_payments[i];
            results->payment_count++;
        }
    }
    
    return SUCCESS;
}

// Generate statistical report
ErrorCode generate_statistical_report(StatisticalReport* report) {
    if (!report) {
        return ERROR_INVALID_INPUT;
    }
    
    // Initialize report
    memset(report, 0, sizeof(StatisticalReport));
    
    // Get all data
    Student students[MAX_STUDENTS];
    int student_count = 0;
    Officer officers[MAX_OFFICERS];
    int officer_count = 0;
    Payment payments[MAX_PAYMENTS];
    int payment_count = 0;
    
    // Collect student statistics
    if (get_all_students(students, MAX_STUDENTS, &student_count) == SUCCESS) {
        report->total_students = student_count;
        
        for (int i = 0; i < student_count; i++) {
            if (strcmp(students[i].status, "Active") == 0) {
                report->active_students++;
            } else if (strcmp(students[i].status, "Inactive") == 0) {
                report->inactive_students++;
            } else if (strcmp(students[i].status, "Graduated") == 0) {
                report->graduated_students++;
            }
        }
    }
    
    // Collect officer statistics
    if (get_all_officers(officers, MAX_OFFICERS, &officer_count) == SUCCESS) {
        report->total_officers = officer_count;
        
        for (int i = 0; i < officer_count; i++) {
            if (strcmp(officers[i].status, "Active") == 0) {
                report->active_officers++;
            }
        }
    }
    
    // Collect payment statistics
    if (get_all_payments(payments, MAX_PAYMENTS, &payment_count) == SUCCESS) {
        report->total_payments = payment_count;
        
        time_t earliest_date = 0;
        time_t latest_date = 0;
        float total_amount = 0.0f;
        
        for (int i = 0; i < payment_count; i++) {
            total_amount += payments[i].amount;
            
            if (strcmp(payments[i].status, "Pending") == 0) {
                report->pending_payments++;
            } else if (strcmp(payments[i].status, "Completed") == 0) {
                report->completed_payments++;
            }
            
            if (earliest_date == 0 || payments[i].paymentDate < earliest_date) {
                earliest_date = payments[i].paymentDate;
            }
            if (payments[i].paymentDate > latest_date) {
                latest_date = payments[i].paymentDate;
            }
        }
        
        report->total_payment_amount = total_amount;
        report->average_payment_amount = payment_count > 0 ? total_amount / payment_count : 0.0f;
        report->earliest_payment_date = earliest_date;
        report->latest_payment_date = latest_date;
    }
    
    return SUCCESS;
}

// Export students to CSV
ErrorCode export_students_to_csv(const Student* students, int count, const char* filename) {
    if (!students || count <= 0 || !filename) {
        return ERROR_INVALID_INPUT;
    }
    
    FILE* file = fopen(filename, "w");
    if (!file) {
        return ERROR_FILE_OPERATION;
    }
    
    // Write CSV header
    fprintf(file, "Student ID,First Name,Last Name,Full Name,Email,Phone,Registration Date,Status\n");
    
    // Write student data
    for (int i = 0; i < count; i++) {
        char date_str[50];
        format_timestamp(students[i].registrationDate, date_str, sizeof(date_str));
        
        fprintf(file, "%s,%s,%s,%s,%s,%s,%s,%s\n",
                students[i].studentID,
                students[i].firstName,
                students[i].lastName,
                students[i].fullName,
                students[i].email,
                students[i].phone,
                date_str,
                students[i].status);
    }
    
    fclose(file);
    return SUCCESS;
}

// Export payments to CSV
ErrorCode export_payments_to_csv(const Payment* payments, int count, const char* filename) {
    if (!payments || count <= 0 || !filename) {
        return ERROR_INVALID_INPUT;
    }
    
    FILE* file = fopen(filename, "w");
    if (!file) {
        return ERROR_FILE_OPERATION;
    }
    
    // Write CSV header
    fprintf(file, "Payment ID,Student ID,Amount,Purpose,Payment Date,Status,Payment Method\n");
    
    // Write payment data
    for (int i = 0; i < count; i++) {
        char date_str[50];
        format_timestamp(payments[i].paymentDate, date_str, sizeof(date_str));
        
        fprintf(file, "%s,%s,%.2f,%s,%s,%s,%s\n",
                payments[i].paymentID,
                payments[i].studentID,
                payments[i].amount,
                payments[i].purpose,
                date_str,
                payments[i].status,
                payments[i].paymentMethod);
    }
    
    fclose(file);
    return SUCCESS;
}

// Display search results
void display_search_results(const SearchResults* results) {
    if (!results) {
        return;
    }
    
    printf("\n=== SEARCH RESULTS ===\n");
    
    if (results->student_count > 0) {
        printf("\n--- Students Found (%d) ---\n", results->student_count);
        display_student_list(results->students, results->student_count);
    }
    
    if (results->officer_count > 0) {
        printf("\n--- Officers Found (%d) ---\n", results->officer_count);
        display_officer_list(results->officers, results->officer_count);
    }
    
    if (results->payment_count > 0) {
        printf("\n--- Payments Found (%d) ---\n", results->payment_count);
        display_payment_list(results->payments, results->payment_count);
    }
    
    if (results->log_count > 0) {
        printf("\n--- Logs Found (%d) ---\n", results->log_count);
        display_log_list(results->logs, results->log_count);
    }
    
    if (results->student_count == 0 && results->officer_count == 0 && 
        results->payment_count == 0 && results->log_count == 0) {
        printf("\nNo records found matching search criteria.\n");
    }
}

// Display statistical report
void display_statistical_report(const StatisticalReport* report) {
    if (!report) {
        return;
    }
    
    printf("\n=== STATISTICAL REPORT ===\n");
    
    printf("\n--- Student Statistics ---\n");
    printf("Total Students: %d\n", report->total_students);
    printf("Active Students: %d\n", report->active_students);
    printf("Inactive Students: %d\n", report->inactive_students);
    printf("Graduated Students: %d\n", report->graduated_students);
    
    printf("\n--- Officer Statistics ---\n");
    printf("Total Officers: %d\n", report->total_officers);
    printf("Active Officers: %d\n", report->active_officers);
    
    printf("\n--- Payment Statistics ---\n");
    printf("Total Payments: %d\n", report->total_payments);
    printf("Total Amount: %.2f\n", report->total_payment_amount);
    printf("Average Payment: %.2f\n", report->average_payment_amount);
    printf("Pending Payments: %d\n", report->pending_payments);
    printf("Completed Payments: %d\n", report->completed_payments);
    
    char earliest_date[50], latest_date[50];
    if (report->earliest_payment_date > 0) {
        format_timestamp(report->earliest_payment_date, earliest_date, sizeof(earliest_date));
        printf("Earliest Payment: %s\n", earliest_date);
    }
    
    if (report->latest_payment_date > 0) {
        format_timestamp(report->latest_payment_date, latest_date, sizeof(latest_date));
        printf("Latest Payment: %s\n", latest_date);
    }
}