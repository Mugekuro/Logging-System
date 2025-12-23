#ifndef PAYMENT_H
#define PAYMENT_H

#include "data_structures.h"
#include <stdbool.h>

// Payment management functions
ErrorCode create_payment(const Payment* payment);
ErrorCode retrieve_payment(const char* payment_id, Payment* payment);
ErrorCode update_payment_record(const Payment* payment);
ErrorCode delete_payment_record(const char* payment_id);
ErrorCode list_all_payments_sorted(Payment** payments, int* count, const char* sort_by); // sort_by: "id", "student_id", "amount", "date"
ErrorCode search_payments_by_student(const char* student_id, Payment** payments, int* count);
ErrorCode search_payments_by_status(const char* status, Payment** payments, int* count);
ErrorCode search_payments_by_date_range(time_t start_date, time_t end_date, Payment** payments, int* count);
ErrorCode get_total_payments_by_student(const char* student_id, float* total_amount);

// Payment validation functions
ErrorCode validate_payment_data(const Payment* payment);
bool is_valid_payment_status(const char* status);

// Payment utility functions
float calculate_total_payments();
int get_payment_count_by_status(const char* status);

#endif // PAYMENT_H