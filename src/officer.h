#ifndef OFFICER_H
#define OFFICER_H

#include "data_structures.h"

// Officer management functions
ErrorCode create_officer(const Officer* officer);
ErrorCode retrieve_officer(const char* officer_id, Officer* officer);
ErrorCode update_officer_record(const Officer* officer);
ErrorCode delete_officer_record(const char* officer_id);
ErrorCode list_all_officers_sorted(Officer** officers, int* count, const char* sort_by); // sort_by: "id", "name", "position"
ErrorCode search_officers_by_name(const char* name, Officer** officers, int* count);
ErrorCode search_officers_by_position(const char* position, Officer** officers, int* count);
ErrorCode search_officers_by_access_level(int access_level, Officer** officers, int* count);

// Officer validation functions
ErrorCode validate_officer_data(const Officer* officer);

// Officer utility functions
int get_total_officer_count();
int get_active_officer_count();

#endif // OFFICER_H