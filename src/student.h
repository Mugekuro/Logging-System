#ifndef STUDENT_H
#define STUDENT_H

#include "data_structures.h"

// Student management functions
ErrorCode create_student(const Student* student);
ErrorCode retrieve_student(const char* student_id, Student* student);
ErrorCode update_student_record(const Student* student);
ErrorCode delete_student_record(const char* student_id);
ErrorCode list_all_students_sorted(Student** students, int* count, const char* sort_by); // sort_by: "id", "name", "date"
ErrorCode search_students_by_name(const char* name, Student** students, int* count);
ErrorCode search_students_by_status(const char* status, Student** students, int* count);

// Student validation functions
ErrorCode validate_student_data(const Student* student);

// Student utility functions
int get_total_student_count();
int get_active_student_count();

#endif // STUDENT_H