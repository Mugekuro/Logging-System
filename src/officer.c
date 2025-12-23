#include "officer.h"
#include "file_io.h"
#include "logging.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Function to create a new officer
ErrorCode create_officer(const Officer* officer) {
    if (!officer) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate officer data
    ErrorCode validation_result = validate_officer_data(officer);
    if (validation_result != SUCCESS) {
        return validation_result;
    }
    
    // Check if officer already exists
    Officer existing_officer;
    ErrorCode load_result = load_officer(officer->officerID, &existing_officer);
    if (load_result == SUCCESS) {
        return ERROR_DUPLICATE_ENTRY;  // Officer already exists
    }
    
    // Set initial values
    ((Officer*)officer)->lastLogin = 0;  // No login yet
    
    // Save to file
    ErrorCode save_result = save_officer(officer);
    if (save_result != SUCCESS) {
        return save_result;
    }
    
    // Log the action
    char log_details[200];
    snprintf(log_details, sizeof(log_details), "Created officer: %s (%s)", 
             officer->officerID, officer->name);
    log_action("SYSTEM", "Officer", "Create", log_details);
    
    return SUCCESS;
}

// Function to retrieve an officer by ID
ErrorCode retrieve_officer(const char* officer_id, Officer* officer) {
    if (!officer_id || !officer) {
        return ERROR_INVALID_INPUT;
    }
    
    ErrorCode result = load_officer(officer_id, officer);
    if (result == SUCCESS) {
        // Log the action
        char log_details[100];
        snprintf(log_details, sizeof(log_details), "Retrieved officer: %s", officer_id);
        log_action("SYSTEM", "Officer", "Retrieve", log_details);
    }
    
    return result;
}

// Function to update an officer record
ErrorCode update_officer_record(const Officer* officer) {
    if (!officer) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate officer data
    ErrorCode validation_result = validate_officer_data(officer);
    if (validation_result != SUCCESS) {
        return validation_result;
    }
    
    // Update in file
    ErrorCode result = update_officer(officer);
    if (result == SUCCESS) {
        // Log the action
        char log_details[200];
        snprintf(log_details, sizeof(log_details), "Updated officer: %s (%s)", 
                 officer->officerID, officer->name);
        log_action("SYSTEM", "Officer", "Update", log_details);
    }
    
    return result;
}

// Function to delete an officer record
ErrorCode delete_officer_record(const char* officer_id) {
    if (!officer_id) {
        return ERROR_INVALID_INPUT;
    }
    
    // Load officer to log the deletion
    Officer officer;
    ErrorCode load_result = load_officer(officer_id, &officer);
    if (load_result != SUCCESS) {
        return load_result;  // Officer not found
    }
    
    // Delete from file
    ErrorCode result = delete_officer(officer_id);
    if (result == SUCCESS) {
        // Log the action
        char log_details[200];
        snprintf(log_details, sizeof(log_details), "Deleted officer: %s (%s)", 
                 officer.officerID, officer.name);
        log_action("SYSTEM", "Officer", "Delete", log_details);
    }
    
    return result;
}

// Function to list all officers sorted by specified field
ErrorCode list_all_officers_sorted(Officer** officers, int* count, const char* sort_by) {
    if (!officers || !count || !sort_by) {
        return ERROR_INVALID_INPUT;
    }
    
    // Get all officers
    ErrorCode result = list_all_officers(officers, count);
    if (result != SUCCESS) {
        return result;
    }
    
    // Sort based on the specified field
    if (strcmp(sort_by, "id") == 0) {
        // Sort by officer ID
        for (int i = 0; i < *count - 1; i++) {
            for (int j = 0; j < *count - i - 1; j++) {
                if (strcmp((*officers)[j].officerID, (*officers)[j+1].officerID) > 0) {
                    // Swap officers
                    Officer temp = (*officers)[j];
                    (*officers)[j] = (*officers)[j+1];
                    (*officers)[j+1] = temp;
                }
            }
        }
    } else if (strcmp(sort_by, "name") == 0) {
        // Sort by name
        for (int i = 0; i < *count - 1; i++) {
            for (int j = 0; j < *count - i - 1; j++) {
                if (strcmp((*officers)[j].name, (*officers)[j+1].name) > 0) {
                    // Swap officers
                    Officer temp = (*officers)[j];
                    (*officers)[j] = (*officers)[j+1];
                    (*officers)[j+1] = temp;
                }
            }
        }
    } else if (strcmp(sort_by, "position") == 0) {
        // Sort by position
        for (int i = 0; i < *count - 1; i++) {
            for (int j = 0; j < *count - i - 1; j++) {
                if (strcmp((*officers)[j].position, (*officers)[j+1].position) > 0) {
                    // Swap officers
                    Officer temp = (*officers)[j];
                    (*officers)[j] = (*officers)[j+1];
                    (*officers)[j+1] = temp;
                }
            }
        }
    } else {
        // Invalid sort field
        return ERROR_INVALID_INPUT;
    }
    
    // Log the action
    char log_details[100];
    snprintf(log_details, sizeof(log_details), "Listed all officers sorted by: %s", sort_by);
    log_action("SYSTEM", "Officer", "List", log_details);
    
    return SUCCESS;
}

// Function to search officers by name
ErrorCode search_officers_by_name(const char* name, Officer** officers, int* count) {
    if (!name || !officers || !count) {
        return ERROR_INVALID_INPUT;
    }
    
    // Get all officers
    Officer* all_officers = NULL;
    int total_count = 0;
    ErrorCode result = list_all_officers(&all_officers, &total_count);
    if (result != SUCCESS) {
        return result;
    }
    
    // Count matching officers
    int matching_count = 0;
    for (int i = 0; i < total_count; i++) {
        if (strstr(all_officers[i].name, name) != NULL) {
            matching_count++;
        }
    }
    
    if (matching_count == 0) {
        safe_free((void**)&all_officers);
        *officers = NULL;
        *count = 0;
        return SUCCESS;
    }
    
    // Create array for matching officers
    Officer* matching_officers = (Officer*)safe_malloc(matching_count * sizeof(Officer));
    if (!matching_officers) {
        safe_free((void**)&all_officers);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    // Copy matching officers
    int index = 0;
    for (int i = 0; i < total_count; i++) {
        if (strstr(all_officers[i].name, name) != NULL) {
            matching_officers[index] = all_officers[i];
            index++;
        }
    }
    
    safe_free((void**)&all_officers);
    *officers = matching_officers;
    *count = matching_count;
    
    // Log the action
    char log_details[100];
    snprintf(log_details, sizeof(log_details), "Searched officers by name: %s", name);
    log_action("SYSTEM", "Officer", "Search", log_details);
    
    return SUCCESS;
}

// Function to search officers by position
ErrorCode search_officers_by_position(const char* position, Officer** officers, int* count) {
    if (!position || !officers || !count) {
        return ERROR_INVALID_INPUT;
    }
    
    // Get all officers
    Officer* all_officers = NULL;
    int total_count = 0;
    ErrorCode result = list_all_officers(&all_officers, &total_count);
    if (result != SUCCESS) {
        return result;
    }
    
    // Count matching officers
    int matching_count = 0;
    for (int i = 0; i < total_count; i++) {
        if (strcmp(all_officers[i].position, position) == 0) {
            matching_count++;
        }
    }
    
    if (matching_count == 0) {
        safe_free((void**)&all_officers);
        *officers = NULL;
        *count = 0;
        return SUCCESS;
    }
    
    // Create array for matching officers
    Officer* matching_officers = (Officer*)safe_malloc(matching_count * sizeof(Officer));
    if (!matching_officers) {
        safe_free((void**)&all_officers);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    // Copy matching officers
    int index = 0;
    for (int i = 0; i < total_count; i++) {
        if (strcmp(all_officers[i].position, position) == 0) {
            matching_officers[index] = all_officers[i];
            index++;
        }
    }
    
    safe_free((void**)&all_officers);
    *officers = matching_officers;
    *count = matching_count;
    
    // Log the action
    char log_details[100];
    snprintf(log_details, sizeof(log_details), "Searched officers by position: %s", position);
    log_action("SYSTEM", "Officer", "Search", log_details);
    
    return SUCCESS;
}

// Function to search officers by access level
ErrorCode search_officers_by_access_level(int access_level, Officer** officers, int* count) {
    if (!officers || !count) {
        return ERROR_INVALID_INPUT;
    }
    
    // Get all officers
    Officer* all_officers = NULL;
    int total_count = 0;
    ErrorCode result = list_all_officers(&all_officers, &total_count);
    if (result != SUCCESS) {
        return result;
    }
    
    // Count matching officers
    int matching_count = 0;
    for (int i = 0; i < total_count; i++) {
        if (all_officers[i].accessLevel == access_level) {
            matching_count++;
        }
    }
    
    if (matching_count == 0) {
        safe_free((void**)&all_officers);
        *officers = NULL;
        *count = 0;
        return SUCCESS;
    }
    
    // Create array for matching officers
    Officer* matching_officers = (Officer*)safe_malloc(matching_count * sizeof(Officer));
    if (!matching_officers) {
        safe_free((void**)&all_officers);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    // Copy matching officers
    int index = 0;
    for (int i = 0; i < total_count; i++) {
        if (all_officers[i].accessLevel == access_level) {
            matching_officers[index] = all_officers[i];
            index++;
        }
    }
    
    safe_free((void**)&all_officers);
    *officers = matching_officers;
    *count = matching_count;
    
    // Log the action
    char log_details[100];
    snprintf(log_details, sizeof(log_details), "Searched officers by access level: %d", access_level);
    log_action("SYSTEM", "Officer", "Search", log_details);
    
    return SUCCESS;
}

// Function to validate officer data
ErrorCode validate_officer_data(const Officer* officer) {
    if (!officer) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate officer ID
    if (!is_valid_id(officer->officerID)) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate name
    if (!is_valid_name(officer->name)) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate position
    if (!is_valid_name(officer->position)) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate access level (1-5 scale)
    if (officer->accessLevel < 1 || officer->accessLevel > 5) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate status
    if (strcmp(officer->status, "Active") != 0 && 
        strcmp(officer->status, "Inactive") != 0) {
        return ERROR_INVALID_INPUT;
    }
    
    return SUCCESS;
}

// Function to get total officer count
int get_total_officer_count() {
    Officer* officers = NULL;
    int count = 0;
    ErrorCode result = list_all_officers(&officers, &count);
    
    if (result == SUCCESS && officers) {
        safe_free((void**)&officers);
    }
    
    return count;
}

// Function to get active officer count
int get_active_officer_count() {
    Officer* officers = NULL;
    int total_count = 0;
    ErrorCode result = list_all_officers(&officers, &total_count);
    
    if (result != SUCCESS) {
        return 0;
    }
    
    int active_count = 0;
    for (int i = 0; i < total_count; i++) {
        if (strcmp(officers[i].status, "Active") == 0) {
            active_count++;
        }
    }
    
    if (officers) {
        safe_free((void**)&officers);
    }
    
    return active_count;
}