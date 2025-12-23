#include "payment.h"
#include "file_io.h"
#include "logging.h"
#include "utils.h"
#include "student.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Function to create a new payment
ErrorCode create_payment(const Payment* payment) {
    if (!payment) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate payment data
    ErrorCode validation_result = validate_payment_data(payment);
    if (validation_result != SUCCESS) {
        return validation_result;
    }
    
    // Check if student exists
    Student student;
    ErrorCode student_result = retrieve_student(payment->studentID, &student);
    if (student_result != SUCCESS) {
        return ERROR_DATA_NOT_FOUND;  // Student not found
    }
    
    // Check if payment already exists
    Payment existing_payment;
    ErrorCode load_result = load_payment(payment->paymentID, &existing_payment);
    if (load_result == SUCCESS) {
        return ERROR_DUPLICATE_ENTRY;  // Payment already exists
    }
    
    // Set payment date if not set
    ((Payment*)payment)->paymentDate = time(NULL);
    
    // Save to file
    ErrorCode save_result = save_payment(payment);
    if (save_result != SUCCESS) {
        return save_result;
    }
    
    // Log the action
    char log_details[200];
    snprintf(log_details, sizeof(log_details), "Created payment: %s for student: %s, amount: %.2f", 
             payment->paymentID, payment->studentID, payment->amount);
    log_action("SYSTEM", "Payment", "Create", log_details);
    
    return SUCCESS;
}

// Function to retrieve a payment by ID
ErrorCode retrieve_payment(const char* payment_id, Payment* payment) {
    if (!payment_id || !payment) {
        return ERROR_INVALID_INPUT;
    }
    
    ErrorCode result = load_payment(payment_id, payment);
    if (result == SUCCESS) {
        // Log the action
        char log_details[100];
        snprintf(log_details, sizeof(log_details), "Retrieved payment: %s", payment_id);
        log_action("SYSTEM", "Payment", "Retrieve", log_details);
    }
    
    return result;
}

// Function to update a payment record
ErrorCode update_payment_record(const Payment* payment) {
    if (!payment) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate payment data
    ErrorCode validation_result = validate_payment_data(payment);
    if (validation_result != SUCCESS) {
        return validation_result;
    }
    
    // Update in file
    ErrorCode result = update_payment(payment);
    if (result == SUCCESS) {
        // Log the action
        char log_details[200];
        snprintf(log_details, sizeof(log_details), "Updated payment: %s for student: %s, amount: %.2f", 
                 payment->paymentID, payment->studentID, payment->amount);
        log_action("SYSTEM", "Payment", "Update", log_details);
    }
    
    return result;
}

// Function to delete a payment record
ErrorCode delete_payment_record(const char* payment_id) {
    if (!payment_id) {
        return ERROR_INVALID_INPUT;
    }
    
    // Load payment to log the deletion
    Payment payment;
    ErrorCode load_result = load_payment(payment_id, &payment);
    if (load_result != SUCCESS) {
        return load_result;  // Payment not found
    }
    
    // Delete from file
    ErrorCode result = delete_payment(payment_id);
    if (result == SUCCESS) {
        // Log the action
        char log_details[200];
        snprintf(log_details, sizeof(log_details), "Deleted payment: %s for student: %s, amount: %.2f", 
                 payment.paymentID, payment.studentID, payment.amount);
        log_action("SYSTEM", "Payment", "Delete", log_details);
    }
    
    return result;
}

// Function to list all payments sorted by specified field
ErrorCode list_all_payments_sorted(Payment** payments, int* count, const char* sort_by) {
    if (!payments || !count || !sort_by) {
        return ERROR_INVALID_INPUT;
    }
    
    // Get all payments
    ErrorCode result = list_all_payments(payments, count);
    if (result != SUCCESS) {
        return result;
    }
    
    // Sort based on the specified field
    if (strcmp(sort_by, "id") == 0) {
        // Sort by payment ID
        for (int i = 0; i < *count - 1; i++) {
            for (int j = 0; j < *count - i - 1; j++) {
                if (strcmp((*payments)[j].paymentID, (*payments)[j+1].paymentID) > 0) {
                    // Swap payments
                    Payment temp = (*payments)[j];
                    (*payments)[j] = (*payments)[j+1];
                    (*payments)[j+1] = temp;
                }
            }
        }
    } else if (strcmp(sort_by, "student_id") == 0) {
        // Sort by student ID
        for (int i = 0; i < *count - 1; i++) {
            for (int j = 0; j < *count - i - 1; j++) {
                if (strcmp((*payments)[j].studentID, (*payments)[j+1].studentID) > 0) {
                    // Swap payments
                    Payment temp = (*payments)[j];
                    (*payments)[j] = (*payments)[j+1];
                    (*payments)[j+1] = temp;
                }
            }
        }
    } else if (strcmp(sort_by, "amount") == 0) {
        // Sort by amount (ascending)
        for (int i = 0; i < *count - 1; i++) {
            for (int j = 0; j < *count - i - 1; j++) {
                if ((*payments)[j].amount > (*payments)[j+1].amount) {
                    // Swap payments
                    Payment temp = (*payments)[j];
                    (*payments)[j] = (*payments)[j+1];
                    (*payments)[j+1] = temp;
                }
            }
        }
    } else if (strcmp(sort_by, "date") == 0) {
        // Sort by date (newest first)
        for (int i = 0; i < *count - 1; i++) {
            for (int j = 0; j < *count - i - 1; j++) {
                if ((*payments)[j].paymentDate < (*payments)[j+1].paymentDate) {
                    // Swap payments
                    Payment temp = (*payments)[j];
                    (*payments)[j] = (*payments)[j+1];
                    (*payments)[j+1] = temp;
                }
            }
        }
    } else {
        // Invalid sort field
        return ERROR_INVALID_INPUT;
    }
    
    // Log the action
    char log_details[100];
    snprintf(log_details, sizeof(log_details), "Listed all payments sorted by: %s", sort_by);
    log_action("SYSTEM", "Payment", "List", log_details);
    
    return SUCCESS;
}

// Function to search payments by student ID
ErrorCode search_payments_by_student(const char* student_id, Payment** payments, int* count) {
    if (!student_id || !payments || !count) {
        return ERROR_INVALID_INPUT;
    }
    
    // Get all payments
    Payment* all_payments = NULL;
    int total_count = 0;
    ErrorCode result = list_all_payments(&all_payments, &total_count);
    if (result != SUCCESS) {
        return result;
    }
    
    // Count matching payments
    int matching_count = 0;
    for (int i = 0; i < total_count; i++) {
        if (strcmp(all_payments[i].studentID, student_id) == 0) {
            matching_count++;
        }
    }
    
    if (matching_count == 0) {
        safe_free((void**)&all_payments);
        *payments = NULL;
        *count = 0;
        return SUCCESS;
    }
    
    // Create array for matching payments
    Payment* matching_payments = (Payment*)safe_malloc(matching_count * sizeof(Payment));
    if (!matching_payments) {
        safe_free((void**)&all_payments);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    // Copy matching payments
    int index = 0;
    for (int i = 0; i < total_count; i++) {
        if (strcmp(all_payments[i].studentID, student_id) == 0) {
            matching_payments[index] = all_payments[i];
            index++;
        }
    }
    
    safe_free((void**)&all_payments);
    *payments = matching_payments;
    *count = matching_count;
    
    // Log the action
    char log_details[100];
    snprintf(log_details, sizeof(log_details), "Searched payments by student: %s", student_id);
    log_action("SYSTEM", "Payment", "Search", log_details);
    
    return SUCCESS;
}

// Function to search payments by status
ErrorCode search_payments_by_status(const char* status, Payment** payments, int* count) {
    if (!status || !payments || !count) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate status
    if (!is_valid_payment_status(status)) {
        return ERROR_INVALID_INPUT;
    }
    
    // Get all payments
    Payment* all_payments = NULL;
    int total_count = 0;
    ErrorCode result = list_all_payments(&all_payments, &total_count);
    if (result != SUCCESS) {
        return result;
    }
    
    // Count matching payments
    int matching_count = 0;
    for (int i = 0; i < total_count; i++) {
        if (strcmp(all_payments[i].status, status) == 0) {
            matching_count++;
        }
    }
    
    if (matching_count == 0) {
        safe_free((void**)&all_payments);
        *payments = NULL;
        *count = 0;
        return SUCCESS;
    }
    
    // Create array for matching payments
    Payment* matching_payments = (Payment*)safe_malloc(matching_count * sizeof(Payment));
    if (!matching_payments) {
        safe_free((void**)&all_payments);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    // Copy matching payments
    int index = 0;
    for (int i = 0; i < total_count; i++) {
        if (strcmp(all_payments[i].status, status) == 0) {
            matching_payments[index] = all_payments[i];
            index++;
        }
    }
    
    safe_free((void**)&all_payments);
    *payments = matching_payments;
    *count = matching_count;
    
    // Log the action
    char log_details[100];
    snprintf(log_details, sizeof(log_details), "Searched payments by status: %s", status);
    log_action("SYSTEM", "Payment", "Search", log_details);
    
    return SUCCESS;
}

// Function to search payments by date range
ErrorCode search_payments_by_date_range(time_t start_date, time_t end_date, Payment** payments, int* count) {
    if (!payments || !count || start_date > end_date) {
        return ERROR_INVALID_INPUT;
    }
    
    // Get all payments
    Payment* all_payments = NULL;
    int total_count = 0;
    ErrorCode result = list_all_payments(&all_payments, &total_count);
    if (result != SUCCESS) {
        return result;
    }
    
    // Count matching payments
    int matching_count = 0;
    for (int i = 0; i < total_count; i++) {
        if (all_payments[i].paymentDate >= start_date && all_payments[i].paymentDate <= end_date) {
            matching_count++;
        }
    }
    
    if (matching_count == 0) {
        safe_free((void**)&all_payments);
        *payments = NULL;
        *count = 0;
        return SUCCESS;
    }
    
    // Create array for matching payments
    Payment* matching_payments = (Payment*)safe_malloc(matching_count * sizeof(Payment));
    if (!matching_payments) {
        safe_free((void**)&all_payments);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    // Copy matching payments
    int index = 0;
    for (int i = 0; i < total_count; i++) {
        if (all_payments[i].paymentDate >= start_date && all_payments[i].paymentDate <= end_date) {
            matching_payments[index] = all_payments[i];
            index++;
        }
    }
    
    safe_free((void**)&all_payments);
    *payments = matching_payments;
    *count = matching_count;
    
    // Log the action
    char start_str[20], end_str[20];
    format_timestamp(start_date, start_str, sizeof(start_str));
    format_timestamp(end_date, end_str, sizeof(end_str));
    char log_details[200];
    snprintf(log_details, sizeof(log_details), "Searched payments from %s to %s", start_str, end_str);
    log_action("SYSTEM", "Payment", "Search", log_details);
    
    return SUCCESS;
}

// Function to get total payments by student
ErrorCode get_total_payments_by_student(const char* student_id, float* total_amount) {
    if (!student_id || !total_amount) {
        return ERROR_INVALID_INPUT;
    }
    
    // Get all payments for this student
    Payment* payments = NULL;
    int count = 0;
    ErrorCode result = search_payments_by_student(student_id, &payments, &count);
    if (result != SUCCESS) {
        return result;
    }
    
    // Calculate total
    float total = 0.0f;
    for (int i = 0; i < count; i++) {
        if (strcmp(payments[i].status, "Completed") == 0) {
            total += payments[i].amount;
        }
    }
    
    *total_amount = total;
    
    if (payments) {
        safe_free((void**)&payments);
    }
    
    // Log the action
    char log_details[100];
    snprintf(log_details, sizeof(log_details), "Calculated total payments for student: %s, amount: %.2f", student_id, total);
    log_action("SYSTEM", "Payment", "Calculate Total", log_details);
    
    return SUCCESS;
}

// Function to validate payment data
ErrorCode validate_payment_data(const Payment* payment) {
    if (!payment) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate payment ID
    if (!is_valid_id(payment->paymentID)) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate student ID
    if (!is_valid_id(payment->studentID)) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate amount
    if (!is_valid_amount(payment->amount)) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate purpose
    if (strlen(payment->purpose) == 0 || strlen(payment->purpose) > MAX_PURPOSE_LEN) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate status
    if (!is_valid_payment_status(payment->status)) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate payment method
    if (strcmp(payment->paymentMethod, "Cash") != 0 && 
        strcmp(payment->paymentMethod, "Card") != 0 && 
        strcmp(payment->paymentMethod, "Online") != 0 &&
        strlen(payment->paymentMethod) > 0) {  // Allow empty for backward compatibility
        return ERROR_INVALID_INPUT;
    }
    
    return SUCCESS;
}

// Function to check if payment status is valid
bool is_valid_payment_status(const char* status) {
    if (!status) {
        return false;
    }
    
    return (strcmp(status, "Pending") == 0 || 
            strcmp(status, "Completed") == 0 || 
            strcmp(status, "Failed") == 0 || 
            strcmp(status, "Refunded") == 0);
}

// Function to calculate total payments
float calculate_total_payments() {
    Payment* payments = NULL;
    int count = 0;
    ErrorCode result = list_all_payments(&payments, &count);
    
    if (result != SUCCESS) {
        return 0.0f;
    }
    
    float total = 0.0f;
    for (int i = 0; i < count; i++) {
        if (strcmp(payments[i].status, "Completed") == 0) {
            total += payments[i].amount;
        }
    }
    
    if (payments) {
        safe_free((void**)&payments);
    }
    
    return total;
}

// Function to get payment count by status
int get_payment_count_by_status(const char* status) {
    if (!status) {
        return 0;
    }
    
    // Validate status
    if (!is_valid_payment_status(status)) {
        return 0;
    }
    
    Payment* payments = NULL;
    int count = 0;
    ErrorCode result = list_all_payments(&payments, &count);
    
    if (result != SUCCESS) {
        return 0;
    }
    
    int status_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(payments[i].status, status) == 0) {
            status_count++;
        }
    }
    
    if (payments) {
        safe_free((void**)&payments);
    }
    
    return status_count;
}