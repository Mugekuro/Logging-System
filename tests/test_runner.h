#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <stdbool.h>

// Test result structure
typedef struct {
    char test_name[100];
    bool passed;
    char error_message[200];
} TestResult;

// Test suite structure
typedef struct {
    char suite_name[100];
    TestResult* results;
    int total_tests;
    int passed_tests;
} TestSuite;

// Test assertion macros
#define ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            current_test->passed = false; \
            snprintf(current_test->error_message, sizeof(current_test->error_message), \
                     "Assertion failed: %s", #condition); \
            return; \
        } \
    } while(0)

#define ASSERT_FALSE(condition) \
    do { \
        if (condition) { \
            current_test->passed = false; \
            snprintf(current_test->error_message, sizeof(current_test->error_message), \
                     "Assertion failed: %s should be false", #condition); \
            return; \
        } \
    } while(0)

#define ASSERT_EQ(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            current_test->passed = false; \
            snprintf(current_test->error_message, sizeof(current_test->error_message), \
                     "Expected %d, got %d", (int)(expected), (int)(actual)); \
            return; \
        } \
    } while(0)

#define ASSERT_STR_EQ(expected, actual) \
    do { \
        if (strcmp((expected), (actual)) != 0) { \
            current_test->passed = false; \
            snprintf(current_test->error_message, sizeof(current_test->error_message), \
                     "Expected '%s', got '%s'", (expected), (actual)); \
            return; \
        } \
    } while(0)

#define ASSERT_NOT_NULL(ptr) \
    do { \
        if ((ptr) == NULL) { \
            current_test->passed = false; \
            snprintf(current_test->error_message, sizeof(current_test->error_message), \
                     "Expected non-NULL pointer"); \
            return; \
        } \
    } while(0)

// Test runner functions
void test_start_suite(const char* suite_name);
void test_start_case(const char* test_name);
void test_end_case();
void test_end_suite();
void test_print_summary();

// External test declarations
void run_all_tests();
void test_auth_module();
void test_student_module();
void test_officer_module();
void test_payment_module();
void test_logging_module();
void test_file_io_module();
void test_utils_module();

#endif // TEST_RUNNER_H