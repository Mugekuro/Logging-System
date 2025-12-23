/**
 * College of Technologies - Student Body Organization
 * 
 * This is a comprehensive management system that helps track:
 * - Student visits and payments
 * - Officer attendance
 * - Payment records for college shirts and other fees
 * - View historical logs by date
 * 
 * This refactored version implements a modular architecture with:
 * - Secure authentication system
 * - Proper data structures
 * - Error handling framework
 * - Logging and audit trail
 * - Input validation
 * - Role-based access control
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Include all module headers
#include "data_structures.h"
#include "auth.h"
#include "student.h"
#include "officer.h"
#include "payment.h"
#include "logging.h"
#include "ui.h"
#include "file_io.h"
#include "utils.h"

// Global variables
static User current_user;

// Function prototypes
void show_main_menu();
void show_student_menu();
void show_officer_menu();
void show_payment_menu();
void show_log_menu();
void handle_student_operations();
void handle_officer_operations();
void handle_payment_operations();
void handle_log_operations();
void handle_student_logging();
void handle_officer_logging();
void handle_payment_processing();
void handle_view_logs();

/**
 * Initialize the system
 */
ErrorCode initialize_system() {
    // Initialize all subsystems
    ErrorCode result;
    
    // Initialize logging system first
    result = init_logging_system();
    if (result != SUCCESS) {
        fprintf(stderr, "Failed to initialize logging system: %s\n", get_error_message(result));
        return result;
    }
    
    // Initialize authentication system
    result = init_auth_system();
    if (result != SUCCESS) {
        log_error(result, "main", "Failed to initialize authentication system");
        return result;
    }
    
    // Log system initialization
    log_action("SYSTEM", "Main", "Initialize", "System initialized successfully");
    
    return SUCCESS;
}

/**
 * Cleanup the system
 */
void cleanup_system() {
    // Log system shutdown
    log_action("SYSTEM", "Main", "Shutdown", "System shutting down");
    
    // Cleanup authentication system
    cleanup_auth_system();
    
    // Cleanup logging system
    cleanup_logging_system();
}

/**
 * Main function - entry point of the application
 */
int main() {
    // Initialize the system
    ErrorCode init_result = initialize_system();
    if (init_result != SUCCESS) {
        fprintf(stderr, "System initialization failed: %s\n", get_error_message(init_result));
        return EXIT_FAILURE;
    }
    
    // Show welcome screen
    clear_screen();
    print_header("College of Technologies - Student Body Organization");
    print_centered_text("Payment and Data Management System");
    print_separator();
    print_centered_text("Welcome to the Student Body Organization Management System");
    print_centered_text("This system provides secure management of students, officers, and payments");
    print_separator();
    
    printf("\nPress Enter to continue...");
    getchar();
    
    // Authentication loop
    bool authenticated = false;
    int login_attempts = 0;
    const int max_attempts = 3;
    
    while (!authenticated && login_attempts < max_attempts) {
        clear_screen();
        print_header("User Authentication");
        
        char username[30];
        char password[MAX_PASSWORD_LEN];
        
        printf("\nEnter Username: ");
        if (fgets(username, sizeof(username), stdin) != NULL) {
            // Remove newline character
            size_t len = strlen(username);
            if (len > 0 && username[len-1] == '\n') {
                username[len-1] = '\0';
            }
        }
        
        printf("Enter Password: ");
        // Simple password masking - in production, use more secure methods
        char ch;
        int i = 0;
        while ((ch = get_char_input()) != '\r' && ch != '\n' && i < MAX_PASSWORD_LEN - 1) {
            if (ch == '\b' && i > 0) {  // Handle backspace
                printf("\b \b");
                i--;
            } else if (ch != '\b') {
                password[i++] = ch;
                printf("*");
            }
        }
        password[i] = '\0';
        printf("\n");
        
        // Authenticate user
        ErrorCode auth_result = authenticate_user(username, password, &current_user);
        if (auth_result == SUCCESS) {
            authenticated = true;
            printf("\nLogin successful! Welcome, %s.\n", current_user.username);
            printf("Press Enter to continue...");
            getchar();
        } else {
            login_attempts++;
            printf("\nLogin failed: %s\n", get_error_message(auth_result));
            if (login_attempts < max_attempts) {
                printf("Attempts remaining: %d\n", max_attempts - login_attempts);
                printf("Press Enter to try again...");
                getchar();
            }
        }
    }
    
    if (!authenticated) {
        printf("\nMaximum login attempts exceeded. Exiting program.\n");
        cleanup_system();
        return EXIT_FAILURE;
    }
    
    // Main program loop
    bool running = true;
    while (running) {
        show_main_menu();
        
        int choice = get_user_choice(1, 5);
        
        switch (choice) {
            case 1:
                handle_student_operations();
                break;
            case 2:
                handle_officer_operations();
                break;
            case 3:
                handle_payment_operations();
                break;
            case 4:
                handle_log_operations();
                break;
            case 5:
                running = false;
                printf("\nLogging out...\n");
                logout_user(&current_user);
                break;
            default:
                print_error("Invalid choice. Please try again.");
                break;
        }
        
        if (running) {
            printf("\nPress Enter to return to the main menu...");
            getchar();
        }
    }
    
    printf("\nThank you for using the Student Body Organization Management System!\n");
    
    // Cleanup and exit
    cleanup_system();
    return EXIT_SUCCESS;
}

/**
 * Show the main menu
 */
void show_main_menu() {
    clear_screen();
    print_header("College of Technologies - Student Body Organization");
    print_centered_text("Main Menu");
    print_separator();
    
    const char* options[] = {
        "[1] Student Operations",
        "[2] Officer Operations", 
        "[3] Payment Operations",
        "[4] View Logs",
        "[5] Exit"
    };
    
    print_menu("Main Menu", options, 5);
    printf("Current User: %s (Role: %s)\n", 
           current_user.username, 
           current_user.role == 1 ? "Admin" : 
           current_user.role == 2 ? "Officer" : "Student");
    print_separator();
}

/**
 * Show the student menu
 */
void show_student_menu() {
    clear_screen();
    print_header("Student Operations");
    print_centered_text("Student Management Menu");
    print_separator();
    
    const char* options[] = {
        "[1] Add New Student",
        "[2] View Student Details",
        "[3] Update Student",
        "[4] Delete Student",
        "[5] List All Students",
        "[6] Search Students",
        "[7] Back to Main Menu"
    };
    
    print_menu("Student Operations", options, 7);
}

/**
 * Show the officer menu
 */
void show_officer_menu() {
    clear_screen();
    print_header("Officer Operations");
    print_centered_text("Officer Management Menu");
    print_separator();
    
    const char* options[] = {
        "[1] Add New Officer",
        "[2] View Officer Details",
        "[3] Update Officer",
        "[4] Delete Officer",
        "[5] List All Officers",
        "[6] Search Officers",
        "[7] Back to Main Menu"
    };
    
    print_menu("Officer Operations", options, 7);
}

/**
 * Show the payment menu
 */
void show_payment_menu() {
    clear_screen();
    print_header("Payment Operations");
    print_centered_text("Payment Management Menu");
    print_separator();
    
    const char* options[] = {
        "[1] Process New Payment",
        "[2] View Payment Details",
        "[3] Update Payment",
        "[4] Delete Payment",
        "[5] List All Payments",
        "[6] Search Payments",
        "[7] Back to Main Menu"
    };
    
    print_menu("Payment Operations", options, 7);
}

/**
 * Show the log menu
 */
void show_log_menu() {
    clear_screen();
    print_header("Log Operations");
    print_centered_text("Log Management Menu");
    print_separator();
    
    const char* options[] = {
        "[1] View Logs by Date",
        "[2] View Logs by User",
        "[3] View Logs by Module",
        "[4] View All Logs",
        "[5] Back to Main Menu"
    };
    
    print_menu("Log Operations", options, 5);
}

/**
 * Handle student operations
 */
void handle_student_operations() {
    bool student_menu_active = true;
    
    while (student_menu_active) {
        show_student_menu();
        
        int choice = get_user_choice(1, 7);
        
        switch (choice) {
            case 1: {  // Add New Student
                if (current_user.role > 2) {  // Only admin/officer can add students
                    print_error("Permission denied. Only admin/officer can add students.");
                    break;
                }
                
                Student new_student = {0};
                
                // Get student information
                get_string_input(new_student.studentID, sizeof(new_student.studentID), "Enter Student ID: ");
                get_string_input(new_student.firstName, sizeof(new_student.firstName), "Enter First Name: ");
                get_string_input(new_student.lastName, sizeof(new_student.lastName), "Enter Last Name: ");
                get_string_input(new_student.email, sizeof(new_student.email), "Enter Email: ");
                get_string_input(new_student.phone, sizeof(new_student.phone), "Enter Phone: ");
                
                // Set status
                strcpy(new_student.status, "Active");
                
                // Create the student
                ErrorCode result = create_student(&new_student);
                if (result == SUCCESS) {
                    print_success("Student created successfully!");
                } else {
                    print_error(get_error_message(result));
                }
                break;
            }
            case 2: {  // View Student Details
                char student_id[20];
                get_string_input(student_id, sizeof(student_id), "Enter Student ID to view: ");
                
                Student student;
                ErrorCode result = retrieve_student(student_id, &student);
                if (result == SUCCESS) {
                    print_separator();
                    display_student(&student);
                    print_separator();
                } else {
                    print_error(get_error_message(result));
                }
                break;
            }
            case 3: {  // Update Student
                if (current_user.role > 2) {  // Only admin/officer can update students
                    print_error("Permission denied. Only admin/officer can update students.");
                    break;
                }
                
                Student student;
                get_string_input(student.studentID, sizeof(student.studentID), "Enter Student ID to update: ");
                
                ErrorCode result = retrieve_student(student.studentID, &student);
                if (result != SUCCESS) {
                    print_error("Student not found!");
                    break;
                }
                
                // Display current information
                printf("Current information:\n");
                display_student(&student);
                
                // Get updated information
                get_string_input(student.firstName, sizeof(student.firstName), "Enter new First Name (or press Enter to keep current): ");
                get_string_input(student.lastName, sizeof(student.lastName), "Enter new Last Name (or press Enter to keep current): ");
                get_string_input(student.email, sizeof(student.email), "Enter new Email (or press Enter to keep current): ");
                get_string_input(student.phone, sizeof(student.phone), "Enter new Phone (or press Enter to keep current): ");
                
                result = update_student_record(&student);
                if (result == SUCCESS) {
                    print_success("Student updated successfully!");
                } else {
                    print_error(get_error_message(result));
                }
                break;
            }
            case 4: {  // Delete Student
                if (current_user.role > 1) {  // Only admin can delete students
                    print_error("Permission denied. Only admin can delete students.");
                    break;
                }
                
                char student_id[20];
                get_string_input(student_id, sizeof(student_id), "Enter Student ID to delete: ");
                
                ErrorCode result = delete_student_record(student_id);
                if (result == SUCCESS) {
                    print_success("Student deleted successfully!");
                } else {
                    print_error(get_error_message(result));
                }
                break;
            }
            case 5: {  // List All Students
                Student* students = NULL;
                int count = 0;
                
                ErrorCode result = list_all_students_sorted(&students, &count, "name");
                if (result == SUCCESS) {
                    print_separator();
                    printf("All Students (%d total):\n", count);
                    display_student_list(students, count);
                    print_separator();
                    
                    if (students) {
                        safe_free((void**)&students);
                    }
                } else {
                    print_error(get_error_message(result));
                }
                break;
            }
            case 6: {  // Search Students
                char search_term[50];
                get_string_input(search_term, sizeof(search_term), "Enter name to search for: ");
                
                Student* students = NULL;
                int count = 0;
                
                ErrorCode result = search_students_by_name(search_term, &students, &count);
                if (result == SUCCESS) {
                    print_separator();
                    printf("Search Results for '%s' (%d matches):\n", search_term, count);
                    display_student_list(students, count);
                    print_separator();
                    
                    if (students) {
                        safe_free((void**)&students);
                    }
                } else {
                    print_error(get_error_message(result));
                }
                break;
            }
            case 7:  // Back to Main Menu
                student_menu_active = false;
                break;
            default:
                print_error("Invalid choice. Please try again.");
                break;
        }
        
        if (student_menu_active && choice != 7) {
            printf("\nPress Enter to continue...");
            getchar();
        }
    }
}

/**
 * Handle officer operations
 */
void handle_officer_operations() {
    bool officer_menu_active = true;
    
    while (officer_menu_active) {
        show_officer_menu();
        
        int choice = get_user_choice(1, 7);
        
        switch (choice) {
            case 1: {  // Add New Officer
                if (current_user.role > 1) {  // Only admin can add officers
                    print_error("Permission denied. Only admin can add officers.");
                    break;
                }
                
                Officer new_officer = {0};
                
                // Get officer information
                get_string_input(new_officer.officerID, sizeof(new_officer.officerID), "Enter Officer ID: ");
                get_string_input(new_officer.name, sizeof(new_officer.name), "Enter Name: ");
                get_string_input(new_officer.position, sizeof(new_officer.position), "Enter Position: ");
                
                // Get access level
                printf("Enter Access Level (1-5): ");
                scanf("%d", &new_officer.accessLevel);
                getchar(); // Consume newline
                
                // Set status
                strcpy(new_officer.status, "Active");
                
                // Create the officer
                ErrorCode result = create_officer(&new_officer);
                if (result == SUCCESS) {
                    print_success("Officer created successfully!");
                } else {
                    print_error(get_error_message(result));
                }
                break;
            }
            case 2: {  // View Officer Details
                char officer_id[20];
                get_string_input(officer_id, sizeof(officer_id), "Enter Officer ID to view: ");
                
                Officer officer;
                ErrorCode result = retrieve_officer(officer_id, &officer);
                if (result == SUCCESS) {
                    print_separator();
                    display_officer(&officer);
                    print_separator();
                } else {
                    print_error(get_error_message(result));
                }
                break;
            }
            case 3: {  // Update Officer
                if (current_user.role > 1) {  // Only admin can update officers
                    print_error("Permission denied. Only admin can update officers.");
                    break;
                }
                
                Officer officer;
                get_string_input(officer.officerID, sizeof(officer.officerID), "Enter Officer ID to update: ");
                
                ErrorCode result = retrieve_officer(officer.officerID, &officer);
                if (result != SUCCESS) {
                    print_error("Officer not found!");
                    break;
                }
                
                // Display current information
                printf("Current information:\n");
                display_officer(&officer);
                
                // Get updated information
                get_string_input(officer.name, sizeof(officer.name), "Enter new Name (or press Enter to keep current): ");
                get_string_input(officer.position, sizeof(officer.position), "Enter new Position (or press Enter to keep current): ");
                
                printf("Enter new Access Level (1-5, or 0 to keep current): ");
                int new_level;
                scanf("%d", &new_level);
                getchar(); // Consume newline
                if (new_level > 0) {
                    officer.accessLevel = new_level;
                }
                
                result = update_officer_record(&officer);
                if (result == SUCCESS) {
                    print_success("Officer updated successfully!");
                } else {
                    print_error(get_error_message(result));
                }
                break;
            }
            case 4: {  // Delete Officer
                if (current_user.role > 1) {  // Only admin can delete officers
                    print_error("Permission denied. Only admin can delete officers.");
                    break;
                }
                
                char officer_id[20];
                get_string_input(officer_id, sizeof(officer_id), "Enter Officer ID to delete: ");
                
                ErrorCode result = delete_officer_record(officer_id);
                if (result == SUCCESS) {
                    print_success("Officer deleted successfully!");
                } else {
                    print_error(get_error_message(result));
                }
                break;
            }
            case 5: {  // List All Officers
                Officer* officers = NULL;
                int count = 0;
                
                ErrorCode result = list_all_officers_sorted(&officers, &count, "name");
                if (result == SUCCESS) {
                    print_separator();
                    printf("All Officers (%d total):\n", count);
                    display_officer_list(officers, count);
                    print_separator();
                    
                    if (officers) {
                        safe_free((void**)&officers);
                    }
                } else {
                    print_error(get_error_message(result));
                }
                break;
            }
            case 6: {  // Search Officers
                char search_term[50];
                get_string_input(search_term, sizeof(search_term), "Enter name to search for: ");
                
                Officer* officers = NULL;
                int count = 0;
                
                ErrorCode result = search_officers_by_name(search_term, &officers, &count);
                if (result == SUCCESS) {
                    print_separator();
                    printf("Search Results for '%s' (%d matches):\n", search_term, count);
                    display_officer_list(officers, count);
                    print_separator();
                    
                    if (officers) {
                        safe_free((void**)&officers);
                    }
                } else {
                    print_error(get_error_message(result));
                }
                break;
            }
            case 7:  // Back to Main Menu
                officer_menu_active = false;
                break;
            default:
                print_error("Invalid choice. Please try again.");
                break;
        }
        
        if (officer_menu_active && choice != 7) {
            printf("\nPress Enter to continue...");
            getchar();
        }
    }
}

/**
 * Handle payment operations
 */
void handle_payment_operations() {
    bool payment_menu_active = true;
    
    while (payment_menu_active) {
        show_payment_menu();
        
        int choice = get_user_choice(1, 7);
        
        switch (choice) {
            case 1: {  // Process New Payment
                if (current_user.role > 2) {  // Only admin/officer can process payments
                    print_error("Permission denied. Only admin/officer can process payments.");
                    break;
                }
                
                Payment new_payment = {0};
                
                // Generate payment ID
                generate_unique_id(new_payment.paymentID, sizeof(new_payment.paymentID), "PAY");
                
                // Get payment information
                get_string_input(new_payment.studentID, sizeof(new_payment.studentID), "Enter Student ID: ");
                new_payment.amount = get_float_input("Enter Amount: $");
                get_string_input(new_payment.purpose, sizeof(new_payment.purpose), "Enter Purpose: ");
                
                // Set status and payment method
                strcpy(new_payment.status, "Completed");
                strcpy(new_payment.paymentMethod, "Cash");  // Default
                
                // Create the payment
                ErrorCode result = create_payment(&new_payment);
                if (result == SUCCESS) {
                    print_success("Payment processed successfully!");
                } else {
                    print_error(get_error_message(result));
                }
                break;
            }
            case 2: {  // View Payment Details
                char payment_id[20];
                get_string_input(payment_id, sizeof(payment_id), "Enter Payment ID to view: ");
                
                Payment payment;
                ErrorCode result = retrieve_payment(payment_id, &payment);
                if (result == SUCCESS) {
                    print_separator();
                    display_payment(&payment);
                    print_separator();
                } else {
                    print_error(get_error_message(result));
                }
                break;
            }
            case 3: {  // Update Payment
                if (current_user.role > 1) {  // Only admin can update payments
                    print_error("Permission denied. Only admin can update payments.");
                    break;
                }
                
                Payment payment;
                get_string_input(payment.paymentID, sizeof(payment.paymentID), "Enter Payment ID to update: ");
                
                ErrorCode result = retrieve_payment(payment.paymentID, &payment);
                if (result != SUCCESS) {
                    print_error("Payment not found!");
                    break;
                }
                
                // Display current information
                printf("Current information:\n");
                display_payment(&payment);
                
                // Get updated information
                printf("Enter new Amount (or 0 to keep current): $");
                float new_amount = get_float_input("");
                if (new_amount > 0) {
                    payment.amount = new_amount;
                }
                
                get_string_input(payment.status, sizeof(payment.status), "Enter new Status (Pending/Completed/Failed/Refunded): ");
                
                result = update_payment_record(&payment);
                if (result == SUCCESS) {
                    print_success("Payment updated successfully!");
                } else {
                    print_error(get_error_message(result));
                }
                break;
            }
            case 4: {  // Delete Payment
                if (current_user.role > 1) {  // Only admin can delete payments
                    print_error("Permission denied. Only admin can delete payments.");
                    break;
                }
                
                char payment_id[20];
                get_string_input(payment_id, sizeof(payment_id), "Enter Payment ID to delete: ");
                
                ErrorCode result = delete_payment_record(payment_id);
                if (result == SUCCESS) {
                    print_success("Payment deleted successfully!");
                } else {
                    print_error(get_error_message(result));
                }
                break;
            }
            case 5: {  // List All Payments
                Payment* payments = NULL;
                int count = 0;
                
                ErrorCode result = list_all_payments_sorted(&payments, &count, "date");
                if (result == SUCCESS) {
                    print_separator();
                    printf("All Payments (%d total):\n", count);
                    display_payment_list(payments, count);
                    print_separator();
                    
                    if (payments) {
                        safe_free((void**)&payments);
                    }
                } else {
                    print_error(get_error_message(result));
                }
                break;
            }
            case 6: {  // Search Payments
                char student_id[20];
                get_string_input(student_id, sizeof(student_id), "Enter Student ID to search for: ");
                
                Payment* payments = NULL;
                int count = 0;
                
                ErrorCode result = search_payments_by_student(student_id, &payments, &count);
                if (result == SUCCESS) {
                    print_separator();
                    printf("Search Results for Student '%s' (%d matches):\n", student_id, count);
                    display_payment_list(payments, count);
                    print_separator();
                    
                    if (payments) {
                        safe_free((void**)&payments);
                    }
                } else {
                    print_error(get_error_message(result));
                }
                break;
            }
            case 7:  // Back to Main Menu
                payment_menu_active = false;
                break;
            default:
                print_error("Invalid choice. Please try again.");
                break;
        }
        
        if (payment_menu_active && choice != 7) {
            printf("\nPress Enter to continue...");
            getchar();
        }
    }
}

/**
 * Handle log operations
 */
void handle_log_operations() {
    bool log_menu_active = true;
    
    while (log_menu_active) {
        show_log_menu();
        
        int choice = get_user_choice(1, 5);
        
        switch (choice) {
            case 1: {  // View Logs by Date
                char date[11];
                get_string_input(date, sizeof(date), "Enter Date (YYYY-MM-DD): ");
                
                LogEntry* logs = NULL;
                int count = 0;
                
                ErrorCode result = get_logs_by_date(date, &logs, &count);
                if (result == SUCCESS) {
                    print_separator();
                    printf("Logs for Date '%s' (%d total):\n", date, count);
                    display_log_list(logs, count);
                    print_separator();
                    
                    if (logs) {
                        safe_free((void**)&logs);
                    }
                } else {
                    print_error(get_error_message(result));
                }
                break;
            }
            case 2: {  // View Logs by User
                char user_id[20];
                get_string_input(user_id, sizeof(user_id), "Enter User ID to view logs for: ");
                
                LogEntry* logs = NULL;
                int count = 0;
                
                ErrorCode result = get_logs_by_user(user_id, &logs, &count);
                if (result == SUCCESS) {
                    print_separator();
                    printf("Logs for User '%s' (%d total):\n", user_id, count);
                    display_log_list(logs, count);
                    print_separator();
                    
                    if (logs) {
                        safe_free((void**)&logs);
                    }
                } else {
                    print_error(get_error_message(result));
                }
                break;
            }
            case 3: {  // View Logs by Module
                char module[20];
                get_string_input(module, sizeof(module), "Enter Module to view logs for (Auth/Student/Officer/Payment): ");
                
                LogEntry* logs = NULL;
                int count = 0;
                
                ErrorCode result = get_logs_by_module(module, &logs, &count);
                if (result == SUCCESS) {
                    print_separator();
                    printf("Logs for Module '%s' (%d total):\n", module, count);
                    display_log_list(logs, count);
                    print_separator();
                    
                    if (logs) {
                        safe_free((void**)&logs);
                    }
                } else {
                    print_error(get_error_message(result));
                }
                break;
            }
            case 4: {  // View All Logs
                LogEntry* logs = NULL;
                int count = 0;
                
                ErrorCode result = load_all_logs(&logs, &count);
                if (result == SUCCESS) {
                    print_separator();
                    printf("All Logs (%d total):\n", count);
                    display_log_list(logs, count);
                    print_separator();
                    
                    if (logs) {
                        safe_free((void**)&logs);
                    }
                } else {
                    print_error(get_error_message(result));
                }
                break;
            }
            case 5:  // Back to Main Menu
                log_menu_active = false;
                break;
            default:
                print_error("Invalid choice. Please try again.");
                break;
        }
        
        if (log_menu_active && choice != 5) {
            printf("\nPress Enter to continue...");
            getchar();
        }
    }
}