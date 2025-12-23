#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <time.h>

#define MAX_ID_LEN 20
#define MAX_NAME_LEN 50
#define MAX_EMAIL_LEN 50
#define MAX_PHONE_LEN 15
#define MAX_STATUS_LEN 15
#define MAX_POSITION_LEN 30
#define MAX_PURPOSE_LEN 100
#define MAX_PAYMENT_METHOD_LEN 20
#define MAX_ACTION_LEN 50
#define MAX_MODULE_LEN 20
#define MAX_DETAILS_LEN 200
#define MAX_PASSWORD_LEN 100

// Student data structure
typedef struct {
    char studentID[MAX_ID_LEN];
    char firstName[30];
    char lastName[30];
    char fullName[61];  // firstName + lastName
    char email[MAX_EMAIL_LEN];
    char phone[MAX_PHONE_LEN];
    time_t registrationDate;
    char status[MAX_STATUS_LEN];    // Active, Inactive, Graduated
} Student;

// Officer data structure
typedef struct {
    char officerID[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    char position[MAX_POSITION_LEN];
    int accessLevel;    // 1-5 scale for role-based access
    time_t lastLogin;
    char status[MAX_STATUS_LEN];    // Active, Inactive
} Officer;

// Payment data structure
typedef struct {
    char paymentID[MAX_ID_LEN];
    char studentID[MAX_ID_LEN];
    float amount;
    char purpose[MAX_PURPOSE_LEN];  // Fee type, shirt purchase, etc.
    time_t paymentDate;
    char status[10];    // Pending, Completed, Failed, Refunded
    char paymentMethod[MAX_PAYMENT_METHOD_LEN]; // Cash, Card, Online
} Payment;

// LogEntry data structure
typedef struct {
    char logID[MAX_ID_LEN];
    char userID[MAX_ID_LEN];
    char action[MAX_ACTION_LEN];    // Login, Create, Update, Delete
    char module[MAX_MODULE_LEN];    // Auth, Student, Officer, Payment
    time_t timestamp;
    char details[MAX_DETAILS_LEN];  // Additional context
} LogEntry;

// User data structure for authentication
typedef struct {
    char userID[MAX_ID_LEN];
    char username[30];
    char password[MAX_PASSWORD_LEN];  // Hashed password
    int role;  // 1=admin, 2=officer, 3=student
    time_t lastLogin;
    char status[MAX_STATUS_LEN];  // Active, Inactive, Locked
} User;

// Error codes
typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_INPUT,
    ERROR_FILE_OPERATION,
    ERROR_AUTH_FAILED,
    ERROR_DATA_NOT_FOUND,
    ERROR_MEMORY_ALLOCATION,
    ERROR_PERMISSION_DENIED,
    ERROR_DUPLICATE_ENTRY
} ErrorCode;

#endif // DATA_STRUCTURES_H