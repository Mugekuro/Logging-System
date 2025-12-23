#ifndef AUTH_H
#define AUTH_H

#include "data_structures.h"
#include "utils.h"

// Authentication functions
ErrorCode init_auth_system();
ErrorCode create_user(const char* username, const char* password, int role);
ErrorCode authenticate_user(const char* username, const char* password, User* user);
ErrorCode change_password(const char* username, const char* old_password, const char* new_password);
ErrorCode logout_user(User* user);
int get_user_role(const char* username);
bool is_user_logged_in(const char* username);
void cleanup_auth_system();

// Session management
typedef struct {
    char session_id[MAX_ID_LEN];
    char user_id[MAX_ID_LEN];
    time_t created_at;
    time_t last_accessed;
    int is_valid;
} Session;

ErrorCode create_session(const char* user_id, Session* session);
ErrorCode validate_session(const char* session_id);
ErrorCode destroy_session(const char* session_id);

#endif // AUTH_H