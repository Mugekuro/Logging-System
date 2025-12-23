#include "utils.h"
#include <time.h>
#include <ctype.h>
#include <stdarg.h>

// Error handling functions
const char* get_error_message(ErrorCode code) {
    switch (code) {
        case SUCCESS:
            return "Success";
        case ERROR_INVALID_INPUT:
            return "Invalid input";
        case ERROR_FILE_OPERATION:
            return "File operation error";
        case ERROR_AUTH_FAILED:
            return "Authentication failed";
        case ERROR_DATA_NOT_FOUND:
            return "Data not found";
        case ERROR_MEMORY_ALLOCATION:
            return "Memory allocation error";
        case ERROR_PERMISSION_DENIED:
            return "Permission denied";
        case ERROR_DUPLICATE_ENTRY:
            return "Duplicate entry";
        default:
            return "Unknown error";
    }
}

void log_error(ErrorCode code, const char* function, const char* message) {
    time_t now;
    char time_str[30];
    
    time(&now);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    fprintf(stderr, "[%s] ERROR in %s: %s (%d: %s)\n", 
            time_str, function, message, code, get_error_message(code));
}

// Input validation functions
bool is_valid_email(const char* email) {
    if (!email || strlen(email) == 0) return false;
    
    int len = strlen(email);
    if (len > MAX_EMAIL_LEN || len < 5) return false;
    
    // Check for @ and .
    bool has_at = false, has_dot = false;
    for (int i = 0; i < len; i++) {
        if (email[i] == '@') {
            has_at = true;
        } else if (email[i] == '.' && i > 0 && email[i-1] != '@') {
            has_dot = true;
        }
    }
    
    return has_at && has_dot;
}

bool is_valid_phone(const char* phone) {
    if (!phone || strlen(phone) == 0) return false;
    
    int len = strlen(phone);
    if (len > MAX_PHONE_LEN || len < 7) return false;
    
    // Check if all characters are digits or common separators
    for (int i = 0; i < len; i++) {
        if (!isdigit(phone[i]) && phone[i] != '-' && phone[i] != ' ' && phone[i] != '(' && phone[i] != ')') {
            return false;
        }
    }
    
    return true;
}

bool is_valid_name(const char* name) {
    if (!name || strlen(name) == 0) return false;
    
    int len = strlen(name);
    if (len > 30 || len < 1) return false;
    
    // Check if all characters are letters, spaces, or hyphens
    for (int i = 0; i < len; i++) {
        if (!isalpha(name[i]) && name[i] != ' ' && name[i] != '-' && name[i] != '\'') {
            return false;
        }
    }
    
    return true;
}

bool is_valid_id(const char* id) {
    if (!id || strlen(id) == 0) return false;
    
    int len = strlen(id);
    if (len >= MAX_ID_LEN || len < 3) return false;
    
    // Check if all characters are alphanumeric
    for (int i = 0; i < len; i++) {
        if (!isalnum(id[i]) && id[i] != '-') {
            return false;
        }
    }
    
    return true;
}

bool is_valid_amount(float amount) {
    return amount >= 0.0f;
}

bool is_valid_date(const char* date) {  // Format: YYYY-MM-DD
    if (!date || strlen(date) != 10) return false;
    
    // Check format: YYYY-MM-DD
    if (date[4] != '-' || date[7] != '-') return false;
    
    // Check if all other characters are digits
    for (int i = 0; i < 10; i++) {
        if (i != 4 && i != 7 && !isdigit(date[i])) {
            return false;
        }
    }
    
    // Parse and validate date components
    int year = atoi(date);
    int month = atoi(date + 5);
    int day = atoi(date + 8);
    
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    
    return true;
}

// String utility functions
char* trim_whitespace(char* str) {
    if (!str) return NULL;
    
    char* start = str;
    char* end = str + strlen(str) - 1;
    
    // Trim leading whitespace
    while (*start && isspace(*start)) {
        start++;
    }
    
    // Trim trailing whitespace
    while (end > start && isspace(*end)) {
        end--;
    }
    
    // Null-terminate the string
    *(end + 1) = '\0';
    
    return start;
}

int safe_strcpy(char* dest, const char* src, size_t dest_size) {
    if (!dest || !src || dest_size == 0) {
        return -1;
    }
    
    size_t src_len = strlen(src);
    if (src_len >= dest_size) {
        src_len = dest_size - 1;
    }
    
    memcpy(dest, src, src_len);
    dest[src_len] = '\0';
    
    return 0;
}

int safe_strcat(char* dest, const char* src, size_t dest_size) {
    if (!dest || !src || dest_size == 0) {
        return -1;
    }
    
    size_t dest_len = strlen(dest);
    size_t src_len = strlen(src);
    
    if (dest_len + src_len >= dest_size) {
        return -1; // Not enough space
    }
    
    strcat(dest, src);
    return 0;
}

// Time utility functions
char* format_timestamp(time_t timestamp, char* buffer, size_t buf_size) {
    if (!buffer || buf_size == 0) return NULL;
    
    struct tm* timeinfo = localtime(&timestamp);
    strftime(buffer, buf_size, "%Y-%m-%d %H:%M:%S", timeinfo);
    return buffer;
}

time_t parse_date_string(const char* date_str) {  // Format: YYYY-MM-DD
    if (!date_str || strlen(date_str) != 10) return -1;
    
    struct tm timeinfo = {0};
    timeinfo.tm_year = atoi(date_str) - 1900;  // Years since 1900
    timeinfo.tm_mon = atoi(date_str + 5) - 1;   // Months since January (0-11)
    timeinfo.tm_mday = atoi(date_str + 8);      // Day of the month (1-31)
    
    return mktime(&timeinfo);
}

// Memory utility functions
void* safe_malloc(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        log_error(ERROR_MEMORY_ALLOCATION, "safe_malloc", "Failed to allocate memory");
    }
    return ptr;
}

void safe_free(void** ptr) {
    if (ptr && *ptr) {
        free(*ptr);
        *ptr = NULL;
    }
}

// Generate unique ID
int generate_unique_id(char* id_buffer, size_t buffer_size, const char* prefix) {
    if (!id_buffer || buffer_size == 0 || !prefix) {
        return -1;
    }
    
    time_t now;
    time(&now);
    
    int prefix_len = strlen(prefix);
    if (prefix_len + 20 >= buffer_size) {  // 20 chars for timestamp + counter
        return -1;
    }
    
    snprintf(id_buffer, buffer_size, "%s%ld", prefix, (long)now);
    
    return 0;
}

// Password utility functions - Simple implementation (in production, use bcrypt or similar)
int hash_password(const char* plain_password, char* hashed_password, size_t hash_size) {
    if (!plain_password || !hashed_password || hash_size < 2) {
        return -1;
    }
    
    // Simple hash function (for demonstration - use bcrypt in production)
    unsigned long hash = 5381;
    int c;
    
    while ((c = *plain_password++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    
    snprintf(hashed_password, hash_size, "%lu", hash);
    return 0;
}

int verify_password(const char* plain_password, const char* hashed_password) {
    if (!plain_password || !hashed_password) {
        return 0;
    }
    
    char temp_hash[MAX_PASSWORD_LEN];
    if (hash_password(plain_password, temp_hash, sizeof(temp_hash)) != 0) {
        return 0;
    }
    
    return strcmp(temp_hash, hashed_password) == 0;
}

// Additional validation functions needed by main.c
bool validate_student_input(const Student *student) {
    if (!student) return false;
    
    // Validate student ID
    if (strlen(student->studentID) == 0 || strlen(student->studentID) >= MAX_ID_LENGTH) {
        return false;
    }
    
    // Validate name
    if (strlen(student->firstName) == 0 || strlen(student->lastName) == 0 ||
        strlen(student->fullName) == 0) {
        return false;
    }
    
    // Validate email format
    if (!is_valid_email(student->email)) {
        return false;
    }
    
    // Validate phone (basic check)
    if (strlen(student->phone) == 0 || strlen(student->phone) > MAX_PHONE_LEN) {
        return false;
    }
    
    // Validate status
    if (strcmp(student->status, "Active") != 0 && 
        strcmp(student->status, "Inactive") != 0 && 
        strcmp(student->status, "Graduated") != 0) {
        return false;
    }
    
    return true;
}

bool validate_officer_input(const Officer *officer) {
    if (!officer) return false;
    
    // Validate officer ID
    if (strlen(officer->officerID) == 0 || strlen(officer->officerID) >= MAX_ID_LENGTH) {
        return false;
    }
    
    // Validate name
    if (strlen(officer->name) == 0 || strlen(officer->name) >= MAX_NAME_LENGTH) {
        return false;
    }
    
    // Validate position
    if (strlen(officer->position) == 0 || strlen(officer->position) >= MAX_POSITION_LENGTH) {
        return false;
    }
    
    // Validate access level
    if (officer->accessLevel < 1 || officer->accessLevel > MAX_ACCESS_LEVEL) {
        return false;
    }
    
    // Validate status
    if (strcmp(officer->status, "Active") != 0 && 
        strcmp(officer->status, "Inactive") != 0) {
        return false;
    }
    
    return true;
}

bool validate_payment_input(const Payment *payment) {
    if (!payment) return false;
    
    // Validate payment ID
    if (strlen(payment->paymentID) == 0 || strlen(payment->paymentID) >= MAX_ID_LENGTH) {
        return false;
    }
    
    // Validate student ID
    if (strlen(payment->studentID) == 0 || strlen(payment->studentID) >= MAX_ID_LENGTH) {
        return false;
    }
    
    // Validate amount
    if (payment->amount <= 0) {
        return false;
    }
    
    // Validate purpose
    if (strlen(payment->purpose) == 0 || strlen(payment->purpose) >= MAX_PURPOSE_LENGTH) {
        return false;
    }
    
    // Validate status
    if (strcmp(payment->status, "Pending") != 0 && 
        strcmp(payment->status, "Completed") != 0 && 
        strcmp(payment->status, "Failed") != 0 && 
        strcmp(payment->status, "Refunded") != 0) {
        return false;
    }
    
    // Validate payment method
    if (strcmp(payment->paymentMethod, "Cash") != 0 && 
        strcmp(payment->paymentMethod, "Card") != 0 && 
        strcmp(payment->paymentMethod, "Online") != 0) {
        return false;
    }
    
    return true;
}

// Date/time utility functions needed by main.c
void getCurrentDateTimeString(char* buffer, size_t buffer_size) {
    time_t now;
    struct tm* timeinfo;
    
    time(&now);
    timeinfo = localtime(&now);
    strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", timeinfo);
}

void getCurrentDateString(char* buffer, size_t buffer_size) {
    time_t now;
    struct tm* timeinfo;
    
    time(&now);
    timeinfo = localtime(&now);
    strftime(buffer, buffer_size, "%Y-%m-%d", timeinfo);
}

bool isValidDate(const char* date_str) {
    if (!date_str || strlen(date_str) != 10) return false;
    
    // Check format YYYY-MM-DD
    if (date_str[4] != '-' || date_str[7] != '-') return false;
    
    // Check if all other characters are digits
    for (int i = 0; i < 10; i++) {
        if (i != 4 && i != 7 && !isdigit(date_str[i])) {
            return false;
        }
    }
    
    // Validate date values
    int year = atoi(date_str);
    int month = atoi(date_str + 5);
    int day = atoi(date_str + 8);
    
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    
    return true;
}