/**
 * College of Technologies - Student Body Organization Management System
 *
 * This is a simple management system that helps track:
 * - Student visits and payments
 * - Officer attendance
 * - Payment records for college shirts and other fees
 * - View historical logs by date
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>  // For getch() function to read keyboard input
#include <time.h>   // For getting current date and time

/**
 * Hides the password as you type by showing asterisks (*) instead
 * This makes it safer when entering passwords
 */
void maskPassword(char password[]);

/**
 * The main program that runs everything
 * Handles login, student records, officer logs, and payments
 */
int main() {
    // Store login information
    char username[20], password[20];
    char correctUser[] = "admin";
    char correctPass[] = "1234";
    int choice;
    char tryAgain;
    int loggedIn = 0;

    // Show welcome screen
    printf("\n\t+------------------------------------------------------+");
    printf("\n\t|                                                      |");
    printf("\n\t|                                                      |");
    printf("\n\t|         Officer and Student Logging System:          |");
    printf("\n\t|             Payment and Data Management              |");
    printf("\n\t|                                                      |");
    printf("\n\t|                                                      |");
    printf("\n\t+------------------------------------------------------+\n");

    printf("\n\tPress Enter to continue...");
    getchar();

    system("cls"); // Clear the screen

    // Keep asking for login until successful or user quits
    do {
        printf("\n\t------------ LOG IN ------------\n");
        printf("\n\tEnter Username: ");
        scanf("%s", username);

        printf("\tEnter Password: ");
        maskPassword(password);
        printf("\n");

        if (strcmp(username, correctUser) == 0 && strcmp(password, correctPass) == 0) {
            printf("\n\t+-----------------------------------------+");
            printf("\n\t|                                         |");
            printf("\n\t|    Login Successful! Welcome, %s.    |", username);
            printf("\n\t|                                         |");
            printf("\n\t+-----------------------------------------+\n");
            loggedIn = 1;
        } else {
            printf("\n\tIncorrect username or password.\n");
            printf("\tDo you want to try again? (Y/N): ");
            scanf(" %c", &tryAgain);
        }

    } while (!loggedIn && (tryAgain == 'y' || tryAgain == 'Y'));

    if (!loggedIn) {
        printf("\n\tExiting program. Goodbye!\n");
        return EXIT_SUCCESS;
    }

    printf("\n\tPress Enter to continue...");
    getchar();
    getchar();

    // Main program loop - keeps running until user chooses to exit
    while (true) {
        system("cls");
        printf(" ==============================    College of Technologies - Student Body Organization     =============================\n\n\n");
        printf("\t----------- Good day, %s! ----------\n", username);
        printf("\n\t+-------------------------------------+");
        printf("\n\t|             MAIN MENU               |");
        printf("\n\t+-------------------------------------+");
        printf("\n\t|                                     |");
        printf("\n\t|       [1] Log as a Student          |");
        printf("\n\t|       [2] Log Officer               |");
        printf("\n\t|       [3] Payment                   |");
        printf("\n\t|       [4] View Log                  |");
        printf("\n\t|       [5] Exit                      |");
        printf("\n\t|                                     |");
        printf("\n\t+-------------------------------------+");
        printf("\n\tChoose (1-5): ");

        choice = getch();

        switch (choice) {
            case '1': {
                // Variables for student information
                char studentID[20], firstName[30], lastName[30], fullName[61];
                char purpose[100];
                char subChoice, paymentChoice, studentType;
                FILE *logFile, *paymentFile, *tempFile;
                time_t now;
                struct tm *local;
                char timeStr[100];
                int studentExists = 0;
                float currentAmountPaid = 0.0;
                float totalAmount = 500.0;  // Cost of college shirt
                char paymentStatus[10];

                system("cls");
                printf(" ==============================    College of Technologies - Student Body Organization     =============================\n\n");
                printf("\n\t------------ STUDENT LOG ------------\n");
                printf("\n\t+-------------------------------------+");
                printf("\n\t|         Student Type                |");
                printf("\n\t+-------------------------------------+");
                printf("\n\t|                                     |");
                printf("\n\t|         [1] New Student             |");
                printf("\n\t|         [2] Existing Student        |");
                printf("\n\t|                                     |");
                printf("\n\t+-------------------------------------+");
                printf("\n\tChoose (1-2): ");
                studentType = getch();

                printf("\n");

                if (studentType == '1') {
                    // Get information for new student
                    printf("\tEnter Student ID: ");
                    scanf("%s", studentID);

                    printf("\tEnter First Name: ");
                    scanf("%s", firstName);

                    printf("\tEnter Last Name: ");
                    scanf("%s", lastName);

                    snprintf(fullName, sizeof(fullName), "%s %s", firstName, lastName);

                    // Check if student is already in the system
                    paymentFile = fopen("payment_records.txt", "r");
                    if (paymentFile) {
                        char line[256];
                        char tempID[20];

                        // Skip the header lines
                        for (int i = 0; i < 4; i++) {
                            if (fgets(line, sizeof(line), paymentFile) == NULL) {
                                break;
                            }
                        }

                        // Look for matching student ID
                        while (fgets(line, sizeof(line), paymentFile)) {
                            sscanf(line, "| %s", tempID);
                            if (strcmp(tempID, studentID) == 0) {
                                studentExists = 1;
                                break;
                            }
                        }
                        fclose(paymentFile);
                    }

                    // Add new student if they don't exist
                    if (!studentExists) {
                        paymentFile = fopen("payment_records.txt", "a+");
                        if (paymentFile) {
                            // Add headers if this is a new file
                            fseek(paymentFile, 0, SEEK_END);
                            long size = ftell(paymentFile);
                            if (size == 0) {
                                fprintf(paymentFile, "\t\t\t\t\tPAYMENT RECORDS\n");
                                fprintf(paymentFile, "+----------------------------------------------------------------------------------------------------------+\n");
                                fprintf(paymentFile, "| %-15s | %-25s | %-10s | %-10s | %-10s |\n", "Student ID", "Name", "Amount Paid", "Total", "Status");
                                fprintf(paymentFile, "+----------------------------------------------------------------------------------------------------------+\n");
                            }

                            // Add the new student's record with proper alignment
                            fprintf(paymentFile, "| %s | %-25s | %10.2f | %-10.2f | %-10s |\n", 
                                   studentID, fullName, 0.00, totalAmount, "Unpaid");
                            fclose(paymentFile);
                            printf("\tNew student record created successfully.\n");
                        } else {
                            printf("\tError: Could not create payment records file.\n");
                        }
                    } else {
                        printf("Student ID already exists in the system.\n");
                    }
                } else if (studentType == '2') {
                    // Handle returning student
                    printf("\tEnter Student ID: ");
                    scanf("%s", studentID);

                    // Look up student information
                    paymentFile = fopen("payment_records.txt", "r");
                    if (paymentFile) {
                        char line[256];
                        char tempID[20], tempName[61];
                        float tempPaid, tempTotal;
                        char tempStatus[10];

                        // Skip the header lines
                        for (int i = 0; i < 4; i++) {
                            if (fgets(line, sizeof(line), paymentFile) == NULL) {
                                break;
                            }
                        }

                        // Find the student's record
                        while (fgets(line, sizeof(line), paymentFile)) {
                            sscanf(line, "| %s | %[^|] | %f | %f | %s |",
                                   tempID, tempName, &tempPaid, &tempTotal, tempStatus);

                            if (strcmp(tempID, studentID) == 0) {
                                // Clean up the name (remove extra spaces)
                                int i = 0, j = strlen(tempName) - 1;
                                while (tempName[i] == ' ') i++;
                                while (tempName[j] == ' ') j--;
                                tempName[j+1] = '\0';
                                strcpy(fullName, tempName + i);

                                currentAmountPaid = tempPaid;
                                studentExists = 1;
                                break;
                            }
                        }
                        fclose(paymentFile);

                        if (studentExists) {
                            printf("\tWelcome back, %s!\n", fullName);
                        } else {
                            printf("\tStudent ID not found. Please enter student details:\n");
                            break;
                        }
                    } else {
                        printf("\tPayment records file does not exist. Creating new record:\n");
                        printf("\tEnter First Name: ");
                        scanf("%s", firstName);

                        printf("\tEnter Last Name: ");
                        scanf("%s", lastName);

                        snprintf(fullName, sizeof(fullName), "%s %s", firstName, lastName);

                        // Create a new payment records file
                        paymentFile = fopen("payment_records.txt", "w");
                        if (paymentFile) {
                            fprintf(paymentFile, "\t\t\t\t\tPAYMENT RECORDS\n");
                            fprintf(paymentFile, "+----------------------------------------------------------------------------------------------------------+\n");
                            fprintf(paymentFile, "| %-15s | %-25s | %-10s | %-10s | %-10s |\n", 
                                   "Student ID", "Name", "Amount Paid", "Total", "Status");
                            fprintf(paymentFile, "+----------------------------------------------------------------------------------------------------------+\n");
                            fprintf(paymentFile, "| %s | %-25s | %10.2f | %-10.2f | %-10s |\n",
                                   studentID, fullName, 0.00, totalAmount, "Unpaid");
                            fclose(paymentFile);
                            printf("\tNew payment records file created with student information.\n");
                        } else {
                            printf("\tError: Could not create payment records file.\n");
                        }
                    }
                } else {
                    printf("\n\tInvalid choice. Returning to main menu.\n");
                    break;
                }

                // Show student options menu
                while (1) {
                    system("cls");
                    printf(" ==============================    College of Technologies - Student Body Organization     =============================\n\n");
                    printf("\n\t ------------ STUDENT LOG ------------- \n\n");
                    printf("\n\tStudent ID: %s", studentID);
                    printf("\n\tName: %s\n", fullName);
                    printf("\n\t+-------------------------------------+");
                    printf("\n\t|         Student Log Options         |");
                    printf("\n\t+-------------------------------------+");
                    printf("\n\t|                                     |");
                    printf("\n\t|         [1] Payment                 |");
                    printf("\n\t|         [2] Sizing                  |");
                    printf("\n\t|         [3] Other Purpose           |");
                    printf("\n\t|         [4] Back to Main Menu       |");
                    printf("\n\t|                                     |");
                    printf("\n\t+-------------------------------------+");

                    printf("\n\tChoose (1-4): ");
                    subChoice = getch();

                    // Get current time for the log
                    time(&now);
                    local = localtime(&now);
                    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", local);

                    // Open the log file
                    logFile = fopen("student_log.txt", "a+");
                    if (logFile) {
                        // Add headers if this is a new file
                        fseek(logFile, 0, SEEK_END);
                        long size = ftell(logFile);
                        if (size == 0) {
                            fprintf(logFile, "\t\t\t\t\tSTUDENT LOGS\n");
                            fprintf(logFile, "+----------------------------------------------------------------------------------------------------------+\n");
                            fprintf(logFile, "| %-20s | %-15s | %-25s | %-35s |\n", "Date & Time", "Student ID", "Name", "Purpose");
                            fprintf(logFile, "|----------------------------------------------------------------------------------------------------------|\n");
                        }

                        switch (subChoice) {
                            case '1': {
                                // Handle payment processing
                                float amount;
                                char tempPurpose[100];

                                printf("\n\t--- Payment Options ---\n");
                                printf("\t1. College Shirt\n");
                                printf("\t2. Other\n");
                                printf("\n\tChoose (1-2): ");
                                paymentChoice = getch();

                                if (paymentChoice == '1') {
                                    printf("\n\tEnter amount to pay: ");
                                    scanf("%f", &amount);
                                    sprintf(purpose, "Payment: College Shirt (%.2f)", amount);

                                    // Update the payment record
                                    float newTotal = currentAmountPaid + amount;

                                    // Update the payment records file
                                    paymentFile = fopen("payment_records.txt", "r");
                                    tempFile = fopen("temp.txt", "w");

                                    if (paymentFile && tempFile) {
                                        char line[256];

                                        // Copy the headers
                                        for (int i = 0; i < 4; i++) {
                                            if (fgets(line, sizeof(line), paymentFile) != NULL) {
                                                fprintf(tempFile, "%s", line);
                                            }
                                        }

                                        // Update the student's record
                                        while (fgets(line, sizeof(line), paymentFile)) {
                                            char tempID[20], tempName[61], tempStatus[10];
                                            float tempPaid, tempTotal;

                                            sscanf(line, "\t| %s | %[^|] | %f | %f | %s |",
                                                  tempID, tempName, &tempPaid, &tempTotal, tempStatus);

                                            if (strcmp(tempID, studentID) == 0) {
                                                tempPaid = newTotal;
                                                strcpy(tempStatus, (tempPaid >= totalAmount) ? "Paid" : "Unpaid");
                                                fprintf(tempFile, "| %-15s | %-25s | %-10.2f | %-10.2f | %-10s |\n",
                                                        tempID, tempName, tempPaid, tempTotal, tempStatus);
                                            } else {
                                                fprintf(tempFile, "%s", line);
                                            }
                                        }

                                        fclose(paymentFile);
                                        fclose(tempFile);

                                        // Replace the old file with the updated one
                                        remove("payment_records.txt");
                                        rename("temp.txt", "payment_records.txt");

                                        currentAmountPaid = newTotal;
                                        printf("\tPayment recorded successfully. New total paid: %.2f\n", newTotal);
                                    } else {
                                        printf("\tError: Could not update payment records.\n");
                                        if (paymentFile) fclose(paymentFile);
                                        if (tempFile) fclose(tempFile);
                                    }
                                } else if (paymentChoice == '2') {
                                    // Handle other types of payments
                                    printf("\n\tEnter Payment Purpose: ");
                                    scanf(" %[^\n]", tempPurpose);
                                    printf("\tEnter amount to pay: ");
                                    scanf("%f", &amount);
                                    sprintf(purpose, "Payment: %s (%.2f)", tempPurpose, amount);

                                    // Update the payment record
                                    float newTotal = currentAmountPaid + amount;

                                    // Update the payment records file
                                    paymentFile = fopen("payment_records.txt", "r");
                                    tempFile = fopen("temp.txt", "w");

                                    if (paymentFile && tempFile) {
                                        char line[256];

                                        // Copy the headers
                                        for (int i = 0; i < 4; i++) {
                                            if (fgets(line, sizeof(line), paymentFile) != NULL) {
                                                fprintf(tempFile, "%s", line);
                                            }
                                        }

                                        // Update the student's record
                                        while (fgets(line, sizeof(line), paymentFile)) {
                                            char tempID[20], tempName[61], tempStatus[10];
                                            float tempPaid, tempTotal;

                                            sscanf(line, "\t| %s | %[^|] | %f | %f | %s |",
                                                  tempID, tempName, &tempPaid, &tempTotal, tempStatus);

                                            if (strcmp(tempID, studentID) == 0) {
                                                tempPaid = newTotal;
                                                strcpy(tempStatus, (tempPaid >= totalAmount) ? "Paid" : "Unpaid");
                                                fprintf(tempFile, "| %-15s | %-25s | %-10.2f | %-10.2f | %-10s |\n",
                                                        tempID, tempName, tempPaid, tempTotal, tempStatus);
                                            } else {
                                                fprintf(tempFile, "%s", line);
                                            }
                                        }

                                        fclose(paymentFile);
                                        fclose(tempFile);

                                        // Replace the old file with the updated one
                                        remove("payment_records.txt");
                                        rename("temp.txt", "payment_records.txt");

                                        currentAmountPaid = newTotal;
                                        printf("\tPayment recorded successfully. New total paid: %.2f\n", newTotal);
                                    } else {
                                        printf("\tError: Could not update payment records.\n");
                                        if (paymentFile) fclose(paymentFile);
                                        if (tempFile) fclose(tempFile);
                                    }
                                } else {
                                    printf("\n\tInvalid payment option.\n");
                                    fclose(logFile);
                                    break;
                                }
                                break;
                            }

                            case '2':
                                // Get t-shirt size
                                printf("\n\tEnter T-shirt Size (e.g., S, M, L, XL): ");
                                scanf("%s", purpose);
                                char temp[4] = "Sizing: ";
                                strcat(temp, purpose);
                                strcpy(purpose, temp);
                                break;

                            case '3':
                                // Get other purpose
                                printf("\n\tEnter Other Purpose: ");
                                scanf(" %[^\n]", purpose);
                                char temp2[50] = "Purpose: ";
                                strcat(temp2, purpose);
                                strcpy(purpose, temp2);
                                break;

                            case '4':
                                fclose(logFile);
                                goto end_student_log;

                            default:
                                printf("\n\tInvalid option. Press any key to try again...");
                                fclose(logFile);
                                getch();
                                continue;
                        }

                        // Save the log entry
                        fprintf(logFile, "| %-20s | %-15s | %-25s | %-35s |\n",
                                timeStr, studentID, fullName, purpose);
                        fclose(logFile);
                        printf("\n\tStudent Logging success at %s\n", timeStr);

                        // Ask if they want to add another entry
                        char again;
                        printf("\n\n\tDo you want to add another purpose for this student? (Y/N): ");
                        scanf(" %c", &again);

                        if (again == 'N' || again == 'n') break;
                    } else {
                        printf("\n\tError: Could not open student_log.txt\n");
                        break;
                    }
                }

                end_student_log:
                break;
            }

            case '2': {
                // Handle officer logging
                char officerID[20], officerName[50];
                FILE *logFile;
                time_t now;
                struct tm *local;
                char timeStr[100];

                system("cls");
                printf(" ==============================    College of Technologies - Student Body Organization     =============================\n\n");
                printf("\n\t ------------ OFFICER LOG ------------- \n\n");

                printf("\tEnter Officer Student ID: ");
                scanf("%s", officerID);

                printf("\tEnter Officer Name: ");
                scanf(" %[^\n]", officerName);

                // Save officer attendance
                logFile = fopen("officer_log.txt", "a+");
                if (logFile) {
                    time(&now);
                    local = localtime(&now);
                    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", local);

                    // Add headers if this is a new file
                    fseek(logFile, 0, SEEK_END);
                    long size = ftell(logFile);
                    if (size == 0) {
                        fprintf(logFile, "\t\t\tOFFICER LOGS\n\n");
                        fprintf(logFile, "+------------------------------------------------------------------------------+\n");
                        fprintf(logFile, "| %-20s | %-28s | %-22s |\n", "Date & Time", "Officer ID", "Name");
                        fprintf(logFile, "|------------------------------------------------------------------------------|\n");
                    }

                    fprintf(logFile, "| %-20s | %-28s | %-22s |\n", timeStr, officerID, officerName);
                    fclose(logFile);

                    printf("\n\tOfficer Logging success at %s\n", timeStr);
                } else {
                    printf("\n\tError: Could not open officer_log.txt\n");
                }

                break;
            }

            case '3': {
                // Show payment records
                FILE *paymentFile;
                char line[256];
                char searchID[20];
                int found = 0;

                system("cls");
                printf(" ==============================    College of Technologies - Student Body Organization     =============================\n\n");
                printf("\n\t\t\t+-------------------------------------+");
                printf("\n\t\t\t|         Payment Records             |");
                printf("\n\t\t\t+-------------------------------------+\n");

                // Open and display payment records
                paymentFile = fopen("payment_records.txt", "r");
                if (paymentFile) {
                    // Skip the header lines
                    for (int i = 0; i < 4; i++) {
                        if (!fgets(line, sizeof(line), paymentFile)) {
                            fclose(paymentFile);
                            break;
                        }
                    }

                    // Show all payment records
                    printf("\n+---------------------------------------------------------------------------------------+\n");
                    printf("| %-15s  | %-25s  | %-10s | %-10s | %-10s |\n", "Student ID", "Name", "Amount Paid", "Total", "Status");
                    printf("|---------------------------------------------------------------------------------------|\n");

                    // Reset to start after headers
                    fseek(paymentFile, 0, SEEK_SET);
                    for (int i = 0; i < 4; i++) {
                        fgets(line, sizeof(line), paymentFile);
                    }

                    // Show all records
                    while (fgets(line, sizeof(line), paymentFile)) {
                        printf("%s", line);
                    }

                    printf("+---------------------------------------------------------------------------------------+\n");

                    // Let user search for a specific student
                    printf("\n\tEnter Student ID to search (or press Enter to exit): ");
                    fflush(stdin);
                    char input = getchar();
                    if (input == '\n') {
                        fclose(paymentFile);
                        break;
                    }
                    ungetc(input, stdin);
                    scanf("%[^\n]", searchID);
                    getchar();

                    // Reset to start after headers
                    fseek(paymentFile, 0, SEEK_SET);
                    for (int i = 0; i < 4; i++) {
                        fgets(line, sizeof(line), paymentFile);
                    }

                    // Look for the student's record
                    while (fgets(line, sizeof(line), paymentFile)) {
                        char currentID[20];
                        sscanf(line, "| %s |", currentID);
                        if (strcmp(currentID, searchID) == 0) {
                            printf("\n\tRecord found:\n");
                            printf("+---------------------------------------------------------------------------------------+\n");
                            printf("| %-15s  | %-25s  | %-10s | %-10s | %-10s |\n",  "Student ID", "Name", "Amount Paid", "Total", "Status");
                            printf("|---------------------------------------------------------------------------------------|\n");
                            printf("%s", line);
                            printf("+---------------------------------------------------------------------------------------+\n");
                            found = 1;
                            break;
                        }
                    }

                    if (!found) {
                        printf("\n\tNo payment records found for Student ID: %s\n", searchID);
                    }

                    fclose(paymentFile);
                } else {
                    printf("\n\tNo payment records found. Please process payment records first.\n");
                }

                break;
            }

            case '4': {
                // Show historical logs
                char subChoice;
                FILE *logFile;
                char line[256];
                char viewDate[11];
                char currentDate[11];
                char viewOption;
                time_t now;
                struct tm *local;

                // Get today's date
                time(&now);
                local = localtime(&now);
                strftime(currentDate, sizeof(currentDate), "%Y-%m-%d", local);

                while (1) {
                    system("cls");
                    printf(" ==============================    College of Technologies - Student Body Organization     =============================\n\n");
                    printf("\n\t+-------------------------------------+");
                    printf("\n\t|              View Logs              |");
                    printf("\n\t+-------------------------------------+");
                    printf("\n\t|                                     |");
                    printf("\n\t|      [1] View Student Logs          |");
                    printf("\n\t|      [2] View Officer Logs          |");
                    printf("\n\t|      [3] Back to Main Menu          |");
                    printf("\n\t|                                     |");
                    printf("\n\t+-------------------------------------+");
                    printf("\n\tChoose (1-3): ");
                    subChoice = getch();

                    switch (subChoice) {
                        case '1': {
                            // Show student logs
                            logFile = fopen("student_log.txt", "r");
                            if (logFile == NULL) {
                                printf("\n\tNo student logs found.\n");
                            } else {
                                // Show the headers
                                printf("\n\n\t--- Date: %s ---\n\n", currentDate);
                                int headerLines = 4;
                                for (int i = 0; i < headerLines; i++) {
                                    if (fgets(line, sizeof(line), logFile)) {
                                        printf("%s", line);
                                    }
                                }

                                // Show today's logs
                                int foundToday = 0;
                                fseek(logFile, 0, SEEK_SET);
                                for (int i = 0; i < headerLines; i++) {
                                    fgets(line, sizeof(line), logFile);
                                }

                                while (fgets(line, sizeof(line), logFile)) {
                                    if (strstr(line, currentDate)) {
                                        printf("%s", line);
                                        foundToday = 1;
                                    }
                                }

                                if (!foundToday) {
                                    printf("No logs found for today (%s).\n", currentDate);
                                }

                                // Let user look up a specific date
                                printf("\n\n\tDo you want to view logs from a specific date? (Y/N): ");
                                scanf(" %c", &viewOption);

                                if (viewOption == 'Y' || viewOption == 'y') {
                                    printf("\tEnter date (YYYY-MM-DD): ");
                                    scanf("%s", viewDate);

                                    fseek(logFile, 0, SEEK_SET);
                                    for (int i = 0; i < headerLines; i++) {
                                        fgets(line, sizeof(line), logFile);
                                        printf("%s", line);
                                    }

                                    int foundSpecific = 0;
                                    while (fgets(line, sizeof(line), logFile)) {
                                        if (strstr(line, viewDate)) {
                                            printf("%s", line);
                                            foundSpecific = 1;
                                        }
                                    }

                                    if (!foundSpecific) {
                                        printf("\tNo logs found for date %s.\n", viewDate);
                                    }
                                }

                                fclose(logFile);
                            }
                            break;
                        }

                        case '2': {
                            // Show officer logs
                            logFile = fopen("officer_log.txt", "r");
                            if (logFile == NULL) {
                                printf("\n\tNo officer logs found.\n");
                            } else {
                                // Show the headers
                                printf("\n\n\t--- Date: %s ---\n\n", currentDate);
                                int headerLines = 5;
                                for (int i = 0; i < headerLines; i++) {
                                    if (fgets(line, sizeof(line), logFile)) {
                                        printf("%s", line);
                                    }
                                }

                                // Show today's logs
                                int foundToday = 0;
                                fseek(logFile, 0, SEEK_SET);
                                for (int i = 0; i < headerLines; i++) {
                                    fgets(line, sizeof(line), logFile);
                                }

                                while (fgets(line, sizeof(line), logFile)) {
                                    if (strstr(line, currentDate)) {
                                        printf("%s", line);
                                        foundToday = 1;
                                    }
                                }

                                if (!foundToday) {
                                    printf("\tNo logs found for today (%s).\n", currentDate);
                                }

                                // Let user look up a specific date
                                printf("\n\tDo you want to view logs from a specific date? (Y/N): ");
                                scanf(" %c", &viewOption);

                                if (viewOption == 'Y' || viewOption == 'y') {
                                    printf("\tEnter date (YYYY-MM-DD): ");
                                    scanf("%s", viewDate);

                                    fseek(logFile, 0, SEEK_SET);
                                    printf("\n\n");
                                    for (int i = 0; i < headerLines; i++) {
                                        fgets(line, sizeof(line), logFile);
                                        printf("%s", line);
                                    }

                                    int foundSpecific = 0;
                                    while (fgets(line, sizeof(line), logFile)) {
                                        if (strstr(line, viewDate)) {
                                            printf("%s", line);
                                            foundSpecific = 1;
                                        }
                                    }

                                    if (!foundSpecific) {
                                        printf("\tNo logs found for date %s.\n", viewDate);
                                    }
                                }

                                fclose(logFile);
                            }
                            break;
                        }

                        case '3':
                            goto end_view_logs;

                        default:
                            printf("\n\tInvalid choice.\n");
                    }

                    printf("\n\tPress any key to continue...");
                    getch();
                }
                end_view_logs:
                break;
            }

            case '5':
                printf("\n\tExiting... Goodbye!\n");
                return EXIT_SUCCESS;

            default:
                printf("\n\tInvalid choice. Try again.\n");
                break;
        }

        printf("\n\tPress any key to return to the menu...");
        getch();
    }

    return EXIT_SUCCESS;
}

/**
 * Hides the password as you type by showing asterisks (*) instead
 * This makes it safer when entering passwords
 */
void maskPassword(char password[]) {
    int i = 0;
    char ch;
    while ((ch = _getch()) != 13 && i < 19) { // Enter key and buffer limit
        if (ch == 8 && i > 0) { // Handle backspace
            printf("\b \b");
            i--;
        } else if (ch != 8) {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
}
