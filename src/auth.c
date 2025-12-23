#include "auth.h"
#include "file_io.h"
#include "logging.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Global variables for authentication system
static User* users = NULL;
static int user_count = 0;
static int max_users = 100;  // Maximum number of users
static Session* sessions = NULL;
static int session_count = 0;
static int max_sessions = 50;  // Maximum number of active sessions

// Function to initialize the authentication system
ErrorCode init_auth_system() {
    // Allocate memory for users array
    users = (User*)safe_malloc(max_users * sizeof(User));
    if (!users) {
        return ERROR_MEMORY_ALLOCATION;
    }
    
    sessions = (Session*)safe_malloc(max_sessions * sizeof(Session));
    if (!sessions) {
        safe_free((void**)&users);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    // Initialize all users as inactive
    for (int i = 0; i < max_users; i++) {
        memset(&users[i], 0, sizeof(User));
        strcpy(users[i].status, "Inactive");
    }
    
    // Initialize all sessions as invalid
    for (int i = 0; i < max_sessions; i++) {
        memset(&sessions[i], 0, sizeof(Session));
        sessions[i].is_valid = 0;
    }
    
    user_count = 0;
    session_count = 0;
    
    // Try to load existing users from file
    FILE* file = fopen("data/users.dat", "rb");
    if (file) {
        // Read user count
        fread(&user_count, sizeof(int), 1, file);
        
        // Read users
        for (int i = 0; i < user_count && i < max_users; i++) {
            fread(&users[i], sizeof(User), 1, file);
        }
        
        fclose(file);
    }
    
    // Create default admin user if no users exist
    if (user_count == 0) {
        ErrorCode result = create_user("admin", "SecurePass123!", 1); // 1 = admin
        if (result != SUCCESS) {
            return result;
        }
    }
    
    return SUCCESS;
}

// Function to create a new user
ErrorCode create_user(const char* username, const char* password, int role) {
    if (!username || !password) {
        return ERROR_INVALID_INPUT;
    }
    
    // Check if username already exists
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return ERROR_DUPLICATE_ENTRY;
        }
    }
    
    // Check if we have space for a new user
    if (user_count >= max_users) {
        return ERROR_MEMORY_ALLOCATION;
    }
    
    // Validate inputs
    if (!is_valid_name(username) || strlen(password) < 6) {
        return ERROR_INVALID_INPUT;
    }
    
    // Create new user
    User* new_user = &users[user_count];
    
    // Generate user ID
    generate_unique_id(new_user->userID, sizeof(new_user->userID), "USR");
    
    safe_strcpy(new_user->username, username, sizeof(new_user->username));
    
    // Hash the password
    if (hash_password(password, new_user->password, sizeof(new_user->password)) != 0) {
        return ERROR_INVALID_INPUT;
    }
    
    new_user->role = role;
    new_user->lastLogin = 0;
    strcpy(new_user->status, "Active");
    
    user_count++;
    
    // Log the user creation
    char log_details[100];
    snprintf(log_details, sizeof(log_details), "Created user: %s with role: %d", username, role);
    log_action("SYSTEM", "Auth", "Create User", log_details);
    
    return SUCCESS;
}

// Function to authenticate a user
ErrorCode authenticate_user(const char* username, const char* password, User* user) {
    if (!username || !password || !user) {
        return ERROR_INVALID_INPUT;
    }
    
    // Find user by username
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            // Check if user is active
            if (strcmp(users[i].status, "Active") != 0) {
                return ERROR_AUTH_FAILED;
            }
            
            // Verify password
            if (verify_password(password, users[i].password)) {
                // Copy user data
                *user = users[i];
                
                // Update last login time
                users[i].lastLogin = time(NULL);
                
                // Log successful login
                char log_details[100];
                snprintf(log_details, sizeof(log_details), "User %s logged in successfully", username);
                log_action(users[i].userID, "Auth", "Login", log_details);
                
                return SUCCESS;
            } else {
                // Log failed login attempt
                char log_details[100];
                snprintf(log_details, sizeof(log_details), "Failed login attempt for user: %s", username);
                log_action("SYSTEM", "Auth", "Login Failed", log_details);
                
                return ERROR_AUTH_FAILED;
            }
        }
    }
    
    // Log failed login attempt
    char log_details[100];
    snprintf(log_details, sizeof(log_details), "Failed login attempt for non-existent user: %s", username);
    log_action("SYSTEM", "Auth", "Login Failed", log_details);
    
    return ERROR_AUTH_FAILED;
}

// Function to change a user's password
ErrorCode change_password(const char* username, const char* old_password, const char* new_password) {
    if (!username || !old_password || !new_password) {
        return ERROR_INVALID_INPUT;
    }
    
    // Find user by username
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            // Verify old password
            if (verify_password(old_password, users[i].password)) {
                // Validate new password (at least 6 characters)
                if (strlen(new_password) < 6) {
                    return ERROR_INVALID_INPUT;
                }
                
                // Hash and update new password
                if (hash_password(new_password, users[i].password, sizeof(users[i].password)) != 0) {
                    return ERROR_INVALID_INPUT;
                }
                
                // Log password change
                char log_details[100];
                snprintf(log_details, sizeof(log_details), "Password changed for user: %s", username);
                log_action(users[i].userID, "Auth", "Password Change", log_details);
                
                return SUCCESS;
            } else {
                return ERROR_AUTH_FAILED;
            }
        }
    }
    
    return ERROR_DATA_NOT_FOUND;
}

// Function to logout a user
ErrorCode logout_user(User* user) {
    if (!user) {
        return ERROR_INVALID_INPUT;
    }
    
    // Log logout
    char log_details[100];
    snprintf(log_details, sizeof(log_details), "User %s logged out", user->username);
    log_action(user->userID, "Auth", "Logout", log_details);
    
    // Clear user data
    memset(user, 0, sizeof(User));
    
    return SUCCESS;
}

// Function to get user role
int get_user_role(const char* username) {
    if (!username) {
        return -1;
    }
    
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return users[i].role;
        }
    }
    
    return -1; // User not found
}

// Function to check if user is logged in
bool is_user_logged_in(const char* username) {
    if (!username) {
        return false;
    }
    
    // In a real implementation, you would check active sessions
    // For this implementation, we'll just check if user exists and is active
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return strcmp(users[i].status, "Active") == 0;
        }
    }
    
    return false;
}

// Function to create a session
ErrorCode create_session(const char* user_id, Session* session) {
    if (!user_id || !session) {
        return ERROR_INVALID_INPUT;
    }
    
    // Check if we have space for a new session
    if (session_count >= max_sessions) {
        return ERROR_MEMORY_ALLOCATION;
    }
    
    // Generate session ID
    generate_unique_id(session->session_id, sizeof(session->session_id), "SES");
    
    // Copy user ID
    safe_strcpy(session->user_id, user_id, sizeof(session->user_id));
    
    // Set session properties
    session->created_at = time(NULL);
    session->last_accessed = session->created_at;
    session->is_valid = 1;
    
    // Store session
    sessions[session_count] = *session;
    session_count++;
    
    return SUCCESS;
}

// Function to validate a session
ErrorCode validate_session(const char* session_id) {
    if (!session_id) {
        return ERROR_INVALID_INPUT;
    }
    
    for (int i = 0; i < session_count; i++) {
        if (sessions[i].is_valid && strcmp(sessions[i].session_id, session_id) == 0) {
            // Update last accessed time
            sessions[i].last_accessed = time(NULL);
            
            // Check if session has expired (30 minutes)
            if (difftime(sessions[i].last_accessed, sessions[i].created_at) > 1800) { // 30 minutes = 1800 seconds
                sessions[i].is_valid = 0;
                return ERROR_AUTH_FAILED;
            }
            
            return SUCCESS;
        }
    }
    
    return ERROR_AUTH_FAILED;
}

// Function to destroy a session
ErrorCode destroy_session(const char* session_id) {
    if (!session_id) {
        return ERROR_INVALID_INPUT;
    }
    
    for (int i = 0; i < session_count; i++) {
        if (strcmp(sessions[i].session_id, session_id) == 0) {
            sessions[i].is_valid = 0;
            memset(&sessions[i], 0, sizeof(Session));
            
            // Shift remaining sessions
            for (int j = i; j < session_count - 1; j++) {
                sessions[j] = sessions[j + 1];
            }
            session_count--;
            
            return SUCCESS;
        }
    }
    
    return ERROR_DATA_NOT_FOUND;
}

// Function to cleanup the authentication system
void cleanup_auth_system() {
    // Save users to file
    FILE* file = fopen("data/users.dat", "wb");
    if (file) {
        // Write user count
        fwrite(&user_count, sizeof(int), 1, file);
        
        // Write users
        for (int i = 0; i < user_count; i++) {
            fwrite(&users[i], sizeof(User), 1, file);
        }
        
        fclose(file);
    }
    
    // Free allocated memory
    safe_free((void**)&users);
    safe_free((void**)&sessions);
}