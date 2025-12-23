#ifndef TYPES_H
#define TYPES_H

#include <time.h>

#define MAX_ID_LENGTH 20
#define MAX_NAME_LENGTH 50
#define MAX_FULLNAME_LENGTH 61
#define MAX_EMAIL_LENGTH 50
#define MAX_PHONE_LENGTH 15
#define MAX_PURPOSE_LENGTH 100
#define MAX_STATUS_LENGTH 10
#define MAX_POSITION_LENGTH 30

typedef struct {
    char studentID[MAX_ID_LENGTH];
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    char fullName[MAX_FULLNAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    time_t registrationDate;
} Student;

typedef struct {
    char officerID[MAX_ID_LENGTH];
    char name[MAX_NAME_LENGTH];
    char position[MAX_POSITION_LENGTH];
    time_t lastLogin;
} Officer;

typedef struct {
    char paymentID[MAX_ID_LENGTH];
    char studentID[MAX_ID_LENGTH];
    float amount;
    char purpose[MAX_PURPOSE_LENGTH];
    time_t paymentDate;
    char status[MAX_STATUS_LENGTH];
} Payment;

typedef struct {
    char logID[MAX_ID_LENGTH];
    char userID[MAX_ID_LENGTH];
    char userType[10];
    char purpose[MAX_PURPOSE_LENGTH];
    time_t logTime;
} LogEntry;

typedef enum {
    SUCCESS,
    ERROR_FILE_NOT_FOUND,
    ERROR_INVALID_INPUT,
    ERROR_DUPLICATE_ENTRY,
    ERROR_MEMORY_ALLOCATION,
    ERROR_PERMISSION_DENIED
} StatusCode;

#endif