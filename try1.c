#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h> // For getch()
#include <time.h>  // For timestamp

// FUNCTION PROTOTYPE
void maskPassword(char password[]);

// MAIN FUNCTION
int main() {
    char username[20], password[20];
    char correctUser[] = "admin";
    char correctPass[] = "1234";
    int choice;
    char tryAgain;
    int loggedIn = 0;


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

    system("cls");

    // LOGIN LOOP
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
            printf("\tDo you want to try again? (y/n): ");
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

    /**

    */


    // MAIN MENU
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
    char studentID[20], firstName[30], lastName[30], fullName[61];
    char purpose[100];
    char subChoice, paymentChoice, studentType;
    FILE *logFile, *paymentFile, *tempFile;
    time_t now;
    struct tm *local;
    char timeStr[100];
    int studentExists = 0;
    float currentAmountPaid = 0.0;
    float totalAmount = 500.0;  // Total amount is always 500
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
        // New student - collect all information
        printf("\tEnter Student ID: ");
        scanf("%s", studentID);

        printf("\tEnter First Name: ");
        scanf("%s", firstName);

        printf("\tEnter Last Name: ");
        scanf("%s", lastName);

        snprintf(fullName, sizeof(fullName), "%s %s", firstName, lastName);

        // Check if student already exists in payment_records.txt
        paymentFile = fopen("payment_records.txt", "r");
        if (paymentFile) {
            char line[256];
            char tempID[20];

            // Skip header lines if they exist
            for (int i = 0; i < 4; i++) {
                if (fgets(line, sizeof(line), paymentFile) == NULL) {
                    break;
                }
            }

            // Check each line for the student ID
            while (fgets(line, sizeof(line), paymentFile)) {
                sscanf(line, "\t| %s", tempID);
                if (strcmp(tempID, studentID) == 0) {
                    studentExists = 1;
                    break;
                }
            }
            fclose(paymentFile);
        }

        // If student doesn't exist, add to payment_records.txt
        if (!studentExists) {
            // Create payment_records.txt if it doesn't exist or append to it
            paymentFile = fopen("payment_records.txt", "a+");
            if (paymentFile) {
                // Check if file is empty to add headers
                fseek(paymentFile, 0, SEEK_END);
                long size = ftell(paymentFile);
                if (size == 0) {
                    fprintf(paymentFile, "\t\t\t\t\tPAYMENT RECORDS\n");
                    fprintf(paymentFile, "|----------------------------------------------------------------------------------------------------------|\n");
                    fprintf(paymentFile, "| %-15s | %-25s | %-10s | %-10s | %-10s |\n", "Student ID", "Name", "Amount Paid", "Total", "Status");
                    fprintf(paymentFile, "|----------------------------------------------------------------------------------------------------------|\n");
                }

                // Add new student with initial payment of 0.00
                fprintf(paymentFile, "| %-15s | %-25s | %-10.2f | %-10.2f | %-10s |\n", studentID, fullName, 0.00, totalAmount, "Unpaid");
                fclose(paymentFile);
                printf("\tNew student record created successfully.\n");
            } else {
                printf("\tError: Could not create payment records file.\n");
            }
        } else {
            printf("Student ID already exists in the system.\n");
        }
    } else if (studentType == '2') {
        // Existing student - just get student ID and retrieve info
        printf("Enter Student ID: ");
        scanf("%s", studentID);

        // Search for student in payment_records.txt
        paymentFile = fopen("payment_records.txt", "r");
        if (paymentFile) {
            char line[256];
            char tempID[20], tempName[61];
            float tempPaid, tempTotal;
            char tempStatus[10];

            // Skip header lines if they exist
            for (int i = 0; i < 4; i++) {
                if (fgets(line, sizeof(line), paymentFile) == NULL) {
                    break;
                }
            }

            // Find student by ID
            while (fgets(line, sizeof(line), paymentFile)) {
                sscanf(line, "| %s | %[^|] | %f | %f | %s |",
                       tempID, tempName, &tempPaid, &tempTotal, tempStatus);

                if (strcmp(tempID, studentID) == 0) {
                    // Remove leading/trailing spaces from name
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
                printf("\tEnter First Name: ");
                scanf("%s", firstName);

                printf("\tEnter Last Name: ");
                scanf("%s", lastName);

                snprintf(fullName, sizeof(fullName), "%s %s", firstName, lastName);

                // Add new student to payment_records.txt
                paymentFile = fopen("payment_records.txt", "a+");
                if (paymentFile) {
                    // Check if file is empty to add headers
                    fseek(paymentFile, 0, SEEK_END);
                    long size = ftell(paymentFile);
                    if (size == 0) {
                        fprintf(paymentFile, "\t\t\t\t\tPAYMENT RECORDS\n");
                        fprintf(paymentFile, "|----------------------------------------------------------------------------------------------------------|\n");
                        fprintf(paymentFile, "| %-15s | %-25s | %-10s | %-10s | %-10s |\n", "Student ID", "Name", "Amount Paid", "Total", "Status");
                        fprintf(paymentFile, "|----------------------------------------------------------------------------------------------------------|\n");
                    }

                    // Add new student with 0.00 payment
                    fprintf(paymentFile, "| %-15s | %-25s | %-10.2f | %-10.2f | %-10s |\n",
                            studentID, fullName, 0.00, totalAmount, "Unpaid");
                    fclose(paymentFile);
                    printf("\tNew student record created.\n");
                } else {
                    printf("\tError: Could not open payment records file.\n");
                }
            }
        } else {
            printf("\tPayment records file does not exist. Creating new record:\n");
            printf("\tEnter First Name: ");
            scanf("%s", firstName);

            printf("\tEnter Last Name: ");
            scanf("%s", lastName);

            snprintf(fullName, sizeof(fullName), "%s %s", firstName, lastName);

            // Create payment_records.txt with headers and first student
            paymentFile = fopen("payment_records.txt", "w");
            if (paymentFile) {
                fprintf(paymentFile, "\t\t\t\t\tPAYMENT RECORDS\n");
                fprintf(paymentFile, "|----------------------------------------------------------------------------------------------------------|\n");
                fprintf(paymentFile, "| %-15s | %-25s | %-10s | %-10s | %-10s |\n", "Student ID", "Name", "Amount Paid", "Total", "Status");
                fprintf(paymentFile, "|----------------------------------------------------------------------------------------------------------|\n");
                fprintf(paymentFile, "| %-15s | %-25s | %-10.2f | %-10.2f | %-10s |\n",
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

        time(&now);
        local = localtime(&now);
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", local);

        logFile = fopen("student_log.txt", "a+");
        if (logFile) {
            // Check if file is empty and add header if needed
            fseek(logFile, 0, SEEK_END);
            long size = ftell(logFile);
            if (size == 0) {
                fprintf(logFile, "\t\t\t\t\tSTUDENT LOGS\n");
                fprintf(logFile, "|----------------------------------------------------------------------------------------------------------|\n");
                fprintf(logFile, "| %-20s | %-15s | %-25s | %-35s |\n", "Date & Time", "Student ID", "Name", "Purpose");
                fprintf(logFile, "|----------------------------------------------------------------------------------------------------------|\n");
            }

            switch (subChoice) {
                    case '1': {
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

                                            // Update payment record in payment_records.txt
                                            float newTotal = currentAmountPaid + amount;

                                            // Read all records into a temporary file
                                            paymentFile = fopen("payment_records.txt", "r");
                                            tempFile = fopen("temp.txt", "w");

                                            if (paymentFile && tempFile) {
                                                char line[256];

                                                // Copy header lines
                                                for (int i = 0; i < 4; i++) {
                                                    if (fgets(line, sizeof(line), paymentFile) != NULL) {
                                                        fprintf(tempFile, "%s", line);
                                                    }
                                                }

                                                // Process each student record
                                                while (fgets(line, sizeof(line), paymentFile)) {
                                                    char tempID[20], tempName[61], tempStatus[10];
                                                    float tempPaid, tempTotal;

                                                    // Extract fields from the line
                                                    sscanf(line, "\t| %s | %[^|] | %f | %f | %s |",
                                                          tempID, tempName, &tempPaid, &tempTotal, tempStatus);

                                                    // If this is our target student, update the amount
                                                    if (strcmp(tempID, studentID) == 0) {
                                                        // Calculate new values
                                                        tempPaid = newTotal;

                                                        // Update payment status
                                                        if (tempPaid >= totalAmount) {
                                                            strcpy(tempStatus, "Paid");
                                                        } else {
                                                            strcpy(tempStatus, "Unpaid");
                                                        }

                                                        // Write updated record
                                                        fprintf(tempFile, "| %-15s | %-25s | %-10.2f | %-10.2f | %-10s |\n",
                                                                tempID, tempName, tempPaid, tempTotal, tempStatus);
                                                    } else {
                                                        // Write original record
                                                        fprintf(tempFile, "%s", line);
                                                    }
                                                }

                                                fclose(paymentFile);
                                                fclose(tempFile);

                                                // Replace original file with updated one
                                                remove("payment_records.txt");
                                                rename("temp.txt", "payment_records.txt");

                                                // Update current amount for this session
                                                currentAmountPaid = newTotal;

                                                printf("\tPayment recorded successfully. New total paid: %.2f\n", newTotal);
                                            } else {
                                                printf("\tError: Could not update payment records.\n");
                                                if (paymentFile) fclose(paymentFile);
                                                if (tempFile) fclose(tempFile);
                                            }
                                        } else if (paymentChoice == '2') {
                                            printf("\n\tEnter Payment Purpose: ");
                                            scanf(" %[^\n]", tempPurpose);
                                            printf("\tEnter amount to pay: ");
                                            scanf("%f", &amount);
                                            sprintf(purpose, "Payment: %s (%.2f)", tempPurpose, amount);

                                            // Update payment record in payment_records.txt
                                            float newTotal = currentAmountPaid + amount;

                                            // Read all records into a temporary file
                                            paymentFile = fopen("payment_records.txt", "r");
                                            tempFile = fopen("temp.txt", "w");

                                            if (paymentFile && tempFile) {
                                                char line[256];

                                                // Copy header lines
                                                for (int i = 0; i < 4; i++) {
                                                    if (fgets(line, sizeof(line), paymentFile) != NULL) {
                                                        fprintf(tempFile, "%s", line);
                                                    }
                                                }

                                                // Process each student record
                                                while (fgets(line, sizeof(line), paymentFile)) {
                                                    char tempID[20], tempName[61], tempStatus[10];
                                                    float tempPaid, tempTotal;

                                                    // Extract fields from the line
                                                    sscanf(line, "\t| %s | %[^|] | %f | %f | %s |",
                                                          tempID, tempName, &tempPaid, &tempTotal, tempStatus);

                                                    // If this is our target student, update the amount
                                                    if (strcmp(tempID, studentID) == 0) {
                                                        // Calculate new values
                                                        tempPaid = newTotal;

                                                        // Update payment status
                                                        if (tempPaid >= totalAmount) {
                                                            strcpy(tempStatus, "Paid");
                                                        } else {
                                                            strcpy(tempStatus, "Unpaid");
                                                        }

                                                        // Write updated record
                                                        fprintf(tempFile, "| %-15s | %-25s | %-10.2f | %-10.2f | %-10s |\n", tempID, tempName, tempPaid, tempTotal, tempStatus);
                                                    } else {
                                                        // Write original record
                                                        fprintf(tempFile, "%s", line);
                                                    }
                                                }

                                                fclose(paymentFile);
                                                fclose(tempFile);

                                                // Replace original file with updated one
                                                remove("payment_records.txt");
                                                rename("temp.txt", "payment_records.txt");

                                                // Update current amount for this session
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
                                        printf("\n\tEnter T-shirt Size (e.g., S, M, L, XL): ");
                                        scanf("%s", purpose);
                                        char temp[100] = "Sizing: ";
                                        strcat(temp, purpose);
                                        strcpy(purpose, temp);
                                        break;

                                    case '3':
                                        printf("\n\tEnter Other Purpose: ");
                                        scanf(" %[^\n]", purpose);
                                        char temp2[100] = "Purpose: ";
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

                                // Write to log file with consistent formatting
                                fprintf(logFile, "| %-20s | %-15s | %-25s | %-35s |\n", timeStr, studentID, fullName, purpose);
                                fclose(logFile);
                                printf("\n\tStudent Logging success at %s\n", timeStr);

                                char again;
                                printf("\n\tDo you want to add another purpose for this student? (Y/N): ");
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

                logFile = fopen("officer_log.txt", "a+");
                if (logFile) {
                    time(&now);
                    local = localtime(&now);
                    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", local);

                    // Check if file is empty
                    fseek(logFile, 0, SEEK_END);
                    long size = ftell(logFile);
                    if (size == 0) {
                        fprintf(logFile, "\t\t\tOFFICER LOGS\n\n");
                        fprintf(logFile, "|------------------------------------------------------------------------------|\n");
                        fprintf(logFile, "| %-20s | %-28s | %-22s |\n", "Date & Time", "Officer ID", "Name");
                        fprintf(logFile, "|------------------------------------------------------------------------------|\n");
                    }

                    fprintf(logFile, "| %-20s | %-28s | %-22s |\n", timeStr, officerID, officerName );
                    fclose(logFile);

                    printf("\n\tOfficer Logging success at %s\n", timeStr);
                } else {
                    printf("\n\tError: Could not open officer_log.txt\n");
                }

                break;
            }

             case '3': {
                    FILE *paymentFile, *studentLogFile;
                    char line[256], viewOption;
                    char tempID[20], tempName[61], tempPurpose[100];
                    float tempAmount, totalAmount = 500.0;  // Total amount is set to 500
                    char paymentStatus[10];
                    time_t now;
                    struct tm *local;
                    char timeStr[100];
                    char viewDate[11]; // YYYY-MM-DD
                    char currentDate[11];
                    char studentPayments[100][3][100]; // Store [studentID][0:totalPaid, 1:name]
                    int studentCount = 0;

                    // Get current date
                    time(&now);
                    local = localtime(&now);
                    strftime(currentDate, sizeof(currentDate), "%Y-%m-%d", local);

                    system("cls");
                    printf(" ==============================    College of Technologies - Student Body Organization     =============================\n\n");
                    printf("\n\t+-------------------------------------+");
                    printf("\n\t|         Payment Records             |");
                    printf("\n\t+-------------------------------------+\n");

                    // Process payment records from student log
                    studentLogFile = fopen("student_log.txt", "r");
                    if (studentLogFile) {
                        // Skip header lines
                        for (int i = 0; i < 4; i++) {
                            if (!fgets(line, sizeof(line), studentLogFile)) {
                                fclose(studentLogFile);
                                break;
                            }
                        }

                        // First pass: collect all payments by student ID
                        while (fgets(line, sizeof(line), studentLogFile)) {
                            // Check if line contains payment information
                            if (strstr(line, "Payment:")) {
                                // Extract data from the log line
                                // Format is "| Date & Time | Student ID | Name | Purpose |"
                                char dateTime[30], studentID[20], name[61], purpose[100];
                                sscanf(line, "| %[^|] | %[^|] | %[^|] | %[^|] |", dateTime, studentID, name, purpose);

                                // Extract amount from purpose (e.g., "Payment: College Shirt (150.00)")
                                char *amountStart = strstr(purpose, "(");
                                char *amountEnd = strstr(purpose, ")");

                                if (amountStart && amountEnd && amountEnd > amountStart) {
                                    char amountStr[20] = {0};
                                    strncpy(amountStr, amountStart + 1, amountEnd - amountStart - 1);
                                    tempAmount = atof(amountStr);

                                    // Check if student ID already exists in our array
                                    int found = -1;
                                    for (int i = 0; i < studentCount; i++) {
                                        if (strcmp(studentPayments[i][0], studentID) == 0) {
                                            found = i;
                                            break;
                                        }
                                    }

                                    if (found >= 0) {
                                        // Update existing record
                                        float currentTotal = atof(studentPayments[found][2]);
                                        float newTotal = currentTotal + tempAmount;
                                        sprintf(studentPayments[found][2], "%.2f", newTotal);
                                    } else {
                                        // Add new record
                                        strcpy(studentPayments[studentCount][0], studentID);
                                        strcpy(studentPayments[studentCount][1], name);
                                        sprintf(studentPayments[studentCount][2], "%.2f", tempAmount);
                                        studentCount++;
                                    }
                                }
                            }
                        }

                        fclose(studentLogFile);

                        // Create payment_records.txt
                        paymentFile = fopen("payment_records.txt", "w");
                        if (paymentFile) {
                            fprintf(paymentFile, "\t\t\t\t\tPAYMENT RECORDS\n");
                            fprintf(paymentFile, "|----------------------------------------------------------------------------------------------------------|\n");
                            fprintf(paymentFile, "| %-15s | %-25s | %-10s | %-10s | %-10s |\n", "Student ID", "Name", "Amount Paid", "Total", "Status");
                            fprintf(paymentFile, "|----------------------------------------------------------------------------------------------------------|\n");

                            // Print header for console display
                            printf("\n|----------------------------------------------------------------------------------------------------------|\n");
                            printf("| %-15s | %-25s | %-10s | %-10s | %-10s |\n", "Student ID", "Name", "Amount Paid", "Total", "Status");
                            printf("|----------------------------------------------------------------------------------------------------------|\n");

                            // Write student payment records
                            for (int i = 0; i < studentCount; i++) {
                                float amountPaid = atof(studentPayments[i][2]);

                                // Determine payment status
                                if (amountPaid >= totalAmount) {
                                    strcpy(paymentStatus, "Paid");
                                } else {
                                    strcpy(paymentStatus, "Unpaid");
                                }

                                // Write to file
                                fprintf(paymentFile, "\t| %-15s | %-25s | %-10.2f | %-10.2f | %-10s |\n",
                                        studentPayments[i][0], studentPayments[i][1], amountPaid, totalAmount, paymentStatus);

                                // Display on console
                                printf("\t| %-15s | %-25s | %-10.2f | %-10.2f | %-10s |\n",
                                        studentPayments[i][0], studentPayments[i][1], amountPaid, totalAmount, paymentStatus);
                            }

                            fprintf(paymentFile, "|----------------------------------------------------------------------------------------------------------|\n");
                            printf("|----------------------------------------------------------------------------------------------------------|\n");

                            fclose(paymentFile);
                        } else {
                            printf("\n\tError: Could not create payment_records.txt\n");
                        }
                    } else {
                        printf("\n\tNo student logs found. Please add student logs first.\n");
                    }

                    printf("\n\tPayment records processed successfully.\n");
                    break;
                }

            case '4': {
                char subChoice;
                FILE *logFile;
                char line[256];
                char viewDate[11]; // YYYY-MM-DD
                char currentDate[11];
                char viewOption;
                time_t now;
                struct tm *local;

                // Get current date
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
                            logFile = fopen("student_log.txt", "r");
                            if (logFile == NULL) {
                                printf("\n\tNo student logs found.\n");
                            } else {
                                // Print header lines
                                printf("\n\n\t--- Date: %s ---\n\n", currentDate);
                                int headerLines = 4; // Title + blank line + header + separator
                                for (int i = 0; i < headerLines; i++) {
                                    if (fgets(line, sizeof(line), logFile)) {
                                        printf("%s", line);
                                    }
                                }

                                // First show today's logs
                                int foundToday = 0;

                                // Reset file position after header
                                fseek(logFile, 0, SEEK_SET);
                                for (int i = 0; i < headerLines; i++) {
                                    fgets(line, sizeof(line), logFile);
                                }

                                while (fgets(line, sizeof(line), logFile)) {
                                    // Check if line contains current date
                                    if (strstr(line, currentDate)) {
                                        printf("%s", line);
                                        foundToday = 1;
                                    }
                                }

                                if (!foundToday) {
                                    printf("No logs found for today (%s).\n", currentDate);
                                }

                                // Ask if user wants to view logs from another date
                                printf("\n\tDo you want to view logs from a specific date? (Y/N): ");
                                scanf(" %c", &viewOption);

                                if (viewOption == 'Y' || viewOption == 'y') {
                                    printf("\tEnter date (YYYY-MM-DD): ");
                                    scanf("%s", viewDate);

                                    // Reset file position after header
                                    fseek(logFile, 0, SEEK_SET);
                                    for (int i = 0; i < headerLines; i++) {
                                        fgets(line, sizeof(line), logFile);
                                        printf("%s", line);
                                    }

                                    int foundSpecific = 0;

                                    while (fgets(line, sizeof(line), logFile)) {
                                        // Check if line contains the specified date
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
                            logFile = fopen("officer_log.txt", "r");
                            if (logFile == NULL) {
                                printf("\n\tNo officer logs found.\n");
                            } else {
                                // Print header lines
                                printf("\n\n\t--- Date: %s ---\n\n", currentDate);
                                int headerLines = 4; // Title + blank line + header + separator
                                for (int i = 0; i < headerLines; i++) {
                                    if (fgets(line, sizeof(line), logFile)) {
                                        printf("%s", line);
                                    }
                                }

                                // First show today's logs
                                int foundToday = 0;

                                // Reset file position after header
                                fseek(logFile, 0, SEEK_SET);
                                for (int i = 0; i < headerLines; i++) {
                                    fgets(line, sizeof(line), logFile);
                                }

                                while (fgets(line, sizeof(line), logFile)) {
                                    // Check if line contains current date
                                    if (strstr(line, currentDate)) {
                                        printf("%s", line); // Removed \t
                                        foundToday = 1;
                                    }
                                }

                                if (!foundToday) {
                                    printf("\tNo logs found for today (%s).\n", currentDate);
                                }

                                // Ask if user wants to view logs from another date
                                printf("\n\tDo you want to view logs from a specific date? (Y/N): ");
                                scanf(" %c", &viewOption);

                                if (viewOption == 'Y' || viewOption == 'y') {
                                    printf("\tEnter date (YYYY-MM-DD): ");
                                    scanf("%s", viewDate);

                                    // Reset file position after header
                                    fseek(logFile, 0, SEEK_SET);
                                    printf("\n\n");
                                    for (int i = 0; i < headerLines; i++) {
                                        fgets(line, sizeof(line), logFile);
                                        printf("%s", line);
                                    }

                                    int foundSpecific = 0;

                                    while (fgets(line, sizeof(line), logFile)) {
                                        // Check if line contains the specified date
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




// FUNCTION DEFINITION
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
