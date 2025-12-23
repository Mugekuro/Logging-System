#include "student.h"
#include "file_io.h"
#include "logging.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Function to create a new student
ErrorCode create_student(const Student* student) {
    if (!student) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate student data
    ErrorCode validation_result = validate_student_data(student);
    if (validation_result != SUCCESS) {
        return validation_result;
    }
    
    // Check if student already exists
    Student existing_student;
    ErrorCode load_result = load_student(student->studentID, &existing_student);
    if (load_result == SUCCESS) {
        return ERROR_DUPLICATE_ENTRY;  // Student already exists
    }
    
    // Set registration date if not set
    ((Student*)student)->registrationDate = time(NULL);
    
    // Save to file
    ErrorCode save_result = save_student(student);
    if (save_result != SUCCESS) {
        return save_result;
    }
    
    // Log the action
    char log_details[200];
    snprintf(log_details, sizeof(log_details), "Created student: %s (%s %s)", 
             student->studentID, student->firstName, student->lastName);
    log_action("SYSTEM", "Student", "Create", log_details);
    
    return SUCCESS;
}

// Function to retrieve a student by ID
ErrorCode retrieve_student(const char* student_id, Student* student) {
    if (!student_id || !student) {
        return ERROR_INVALID_INPUT;
    }
    
    ErrorCode result = load_student(student_id, student);
    if (result == SUCCESS) {
        // Log the action
        char log_details[100];
        snprintf(log_details, sizeof(log_details), "Retrieved student: %s", student_id);
        log_action("SYSTEM", "Student", "Retrieve", log_details);
    }
    
    return result;
}

// Function to update a student record
ErrorCode update_student_record(const Student* student) {
    if (!student) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate student data
    ErrorCode validation_result = validate_student_data(student);
    if (validation_result != SUCCESS) {
        return validation_result;
    }
    
    // Update in file
    ErrorCode result = update_student(student);
    if (result == SUCCESS) {
        // Log the action
        char log_details[200];
        snprintf(log_details, sizeof(log_details), "Updated student: %s (%s %s)", 
                 student->studentID, student->firstName, student->lastName);
        log_action("SYSTEM", "Student", "Update", log_details);
    }
    
    return result;
}

// Function to delete a student record
ErrorCode delete_student_record(const char* student_id) {
    if (!student_id) {
        return ERROR_INVALID_INPUT;
    }
    
    // Load student to log the deletion
    Student student;
    ErrorCode load_result = load_student(student_id, &student);
    if (load_result != SUCCESS) {
        return load_result;  // Student not found
    }
    
    // Delete from file
    ErrorCode result = delete_student(student_id);
    if (result == SUCCESS) {
        // Log the action
        char log_details[200];
        snprintf(log_details, sizeof(log_details), "Deleted student: %s (%s %s)", 
                 student.studentID, student.firstName, student.lastName);
        log_action("SYSTEM", "Student", "Delete", log_details);
    }
    
    return result;
}

// Function to list all students sorted by specified field
ErrorCode list_all_students_sorted(Student** students, int* count, const char* sort_by) {
    if (!students || !count || !sort_by) {
        return ERROR_INVALID_INPUT;
    }
    
    // Get all students
    ErrorCode result = list_all_students(students, count);
    if (result != SUCCESS) {
        return result;
    }
    
    // Sort based on the specified field
    if (strcmp(sort_by, "id") == 0) {
        // Sort by student ID
        for (int i = 0; i < *count - 1; i++) {
            for (int j = 0; j < *count - i - 1; j++) {
                if (strcmp((*students)[j].studentID, (*students)[j+1].studentID) > 0) {
                    // Swap students
                    Student temp = (*students)[j];
                    (*students)[j] = (*students)[j+1];
                    (*students)[j+1] = temp;
                }
            }
        }
    } else if (strcmp(sort_by, "name") == 0) {
        // Sort by full name
        for (int i = 0; i < *count - 1; i++) {
            for (int j = 0; j < *count - i - 1; j++) {
                if (strcmp((*students)[j].fullName, (*students)[j+1].fullName) > 0) {
                    // Swap students
                    Student temp = (*students)[j];
                    (*students)[j] = (*students)[j+1];
                    (*students)[j+1] = temp;
                }
            }
        }
    } else if (strcmp(sort_by, "date") == 0) {
        // Sort by registration date (newest first)
        for (int i = 0; i < *count - 1; i++) {
            for (int j = 0; j < *count - i - 1; j++) {
                if ((*students)[j].registrationDate < (*students)[j+1].registrationDate) {
                    // Swap students
                    Student temp = (*students)[j];
                    (*students)[j] = (*students)[j+1];
                    (*students)[j+1] = temp;
                }
            }
        }
    } else {
        // Invalid sort field
        return ERROR_INVALID_INPUT;
    }
    
    // Log the action
    char log_details[100];
    snprintf(log_details, sizeof(log_details), "Listed all students sorted by: %s", sort_by);
    log_action("SYSTEM", "Student", "List", log_details);
    
    return SUCCESS;
}

// Function to search students by name
ErrorCode search_students_by_name(const char* name, Student** students, int* count) {
    if (!name || !students || !count) {
        return ERROR_INVALID_INPUT;
    }
    
    // Get all students
    Student* all_students = NULL;
    int total_count = 0;
    ErrorCode result = list_all_students(&all_students, &total_count);
    if (result != SUCCESS) {
        return result;
    }
    
    // Count matching students
    int matching_count = 0;
    for (int i = 0; i < total_count; i++) {
        if (strstr(all_students[i].fullName, name) != NULL) {
            matching_count++;
        }
    }
    
    if (matching_count == 0) {
        safe_free((void**)&all_students);
        *students = NULL;
        *count = 0;
        return SUCCESS;
    }
    
    // Create array for matching students
    Student* matching_students = (Student*)safe_malloc(matching_count * sizeof(Student));
    if (!matching_students) {
        safe_free((void**)&all_students);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    // Copy matching students
    int index = 0;
    for (int i = 0; i < total_count; i++) {
        if (strstr(all_students[i].fullName, name) != NULL) {
            matching_students[index] = all_students[i];
            index++;
        }
    }
    
    safe_free((void**)&all_students);
    *students = matching_students;
    *count = matching_count;
    
    // Log the action
    char log_details[100];
    snprintf(log_details, sizeof(log_details), "Searched students by name: %s", name);
    log_action("SYSTEM", "Student", "Search", log_details);
    
    return SUCCESS;
}

// Function to search students by status
ErrorCode search_students_by_status(const char* status, Student** students, int* count) {
    if (!status || !students || !count) {
        return ERROR_INVALID_INPUT;
    }
    
    // Get all students
    Student* all_students = NULL;
    int total_count = 0;
    ErrorCode result = list_all_students(&all_students, &total_count);
    if (result != SUCCESS) {
        return result;
    }
    
    // Count matching students
    int matching_count = 0;
    for (int i = 0; i < total_count; i++) {
        if (strcmp(all_students[i].status, status) == 0) {
            matching_count++;
        }
    }
    
    if (matching_count == 0) {
        safe_free((void**)&all_students);
        *students = NULL;
        *count = 0;
        return SUCCESS;
    }
    
    // Create array for matching students
    Student* matching_students = (Student*)safe_malloc(matching_count * sizeof(Student));
    if (!matching_students) {
        safe_free((void**)&all_students);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    // Copy matching students
    int index = 0;
    for (int i = 0; i < total_count; i++) {
        if (strcmp(all_students[i].status, status) == 0) {
            matching_students[index] = all_students[i];
            index++;
        }
    }
    
    safe_free((void**)&all_students);
    *students = matching_students;
    *count = matching_count;
    
    // Log the action
    char log_details[100];
    snprintf(log_details, sizeof(log_details), "Searched students by status: %s", status);
    log_action("SYSTEM", "Student", "Search", log_details);
    
    return SUCCESS;
}

// Function to validate student data
ErrorCode validate_student_data(const Student* student) {
    if (!student) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate student ID
    if (!is_valid_id(student->studentID)) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate names
    if (!is_valid_name(student->firstName) || !is_valid_name(student->lastName)) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate email
    if (!is_valid_email(student->email)) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate phone
    if (!is_valid_phone(student->phone)) {
        return ERROR_INVALID_INPUT;
    }
    
    // Validate status
    if (strcmp(student->status, "Active") != 0 && 
        strcmp(student->status, "Inactive") != 0 && 
        strcmp(student->status, "Graduated") != 0) {
        return ERROR_INVALID_INPUT;
    }
    
    // Create full name
    snprintf((char*)student->fullName, sizeof(student->fullName), "%s %s", 
             student->firstName, student->lastName);
    
    return SUCCESS;
}

// Function to get total student count
int get_total_student_count() {
    Student* students = NULL;
    int count = 0;
    ErrorCode result = list_all_students(&students, &count);
    
    if (result == SUCCESS && students) {
        safe_free((void**)&students);
    }
    
    return count;
}

// Function to get active student count
int get_active_student_count() {
    Student* students = NULL;
    int total_count = 0;
    ErrorCode result = list_all_students(&students, &total_count);
    
    if (result != SUCCESS) {
        return 0;
    }
    
    int active_count = 0;
    for (int i = 0; i < total_count; i++) {
        if (strcmp(students[i].status, "Active") == 0) {
            active_count++;
        }
    }
    
    if (students) {
        safe_free((void**)&students);
    }
    
    return active_count;
}