#ifndef SEARCH_REPORT_H
#define SEARCH_REPORT_H

#include "data_structures.h"

// Search criteria structure for students
typedef struct {
    char student_id[MAX_ID_LENGTH];
    char first_name[MAX_FIRST_NAME_LENGTH];
    char last_name[MAX_LAST_NAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char status[MAX_STATUS_LENGTH];
    time_t registration_date_from;
    time_t registration_date_to;
    bool use_registration_date_range;
} StudentSearchCriteria;

// Search criteria structure for officers
typedef struct {
    char officer_id[MAX_ID_LENGTH];
    char name[MAX_NAME_LENGTH];
    char position[MAX_POSITION_LENGTH];
    int access_level_min;
    int access_level_max;
    time_t last_login_from;
    time_t last_login_to;
    bool use_access_level_range;
    bool use_last_login_range;
} OfficerSearchCriteria;

// Search criteria structure for payments
typedef struct {
    char payment_id[MAX_ID_LENGTH];
    char student_id[MAX_ID_LENGTH];
    char purpose[MAX_PURPOSE_LENGTH];
    float amount_min;
    float amount_max;
    time_t payment_date_from;
    time_t payment_date_to;
    char payment_method[MAX_PAYMENT_METHOD_LENGTH];
    char status[MAX_STATUS_LENGTH];
    bool use_amount_range;
    bool use_date_range;
} PaymentSearchCriteria;

// Search results structure
typedef struct {
    Student* students;
    int student_count;
    Officer* officers;
    int officer_count;
    Payment* payments;
    int payment_count;
    LogEntry* logs;
    int log_count;
} SearchResults;

// Statistical report structure
typedef struct {
    int total_students;
    int active_students;
    int inactive_students;
    int graduated_students;
    int total_officers;
    int active_officers;
    int total_payments;
    float total_payment_amount;
    float average_payment_amount;
    int pending_payments;
    int completed_payments;
    time_t earliest_payment_date;
    time_t latest_payment_date;
} StatisticalReport;

// Search functions
ErrorCode search_students(const StudentSearchCriteria* criteria, SearchResults* results);
ErrorCode search_officers(const OfficerSearchCriteria* criteria, SearchResults* results);
ErrorCode search_payments(const PaymentSearchCriteria* criteria, SearchResults* results);
ErrorCode search_logs(const LogEntry* log_criteria, SearchResults* results);

// Reporting functions
ErrorCode generate_statistical_report(StatisticalReport* report);
ErrorCode generate_payment_summary(time_t start_date, time_t end_date, SearchResults* results);
ErrorCode generate_attendance_report(time_t start_date, time_t end_date, SearchResults* results);

// Export functions
ErrorCode export_students_to_csv(const Student* students, int count, const char* filename);
ErrorCode export_payments_to_csv(const Payment* payments, int count, const char* filename);
ErrorCode export_data_to_json(const SearchResults* results, const char* filename);

// Display functions
void display_search_results(const SearchResults* results);
void display_statistical_report(const StatisticalReport* report);

#endif // SEARCH_REPORT_H