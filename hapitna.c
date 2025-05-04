#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h> // For getch()
#include <time.h>  // For timestamp

// FUNCTION PROTOTYPES
void maskPassword(char password[]);
int getValidInput(char *prompt, char *buffer, int bufferSize);
int getValidChoice(char *prompt, char validChoices[], int numChoices);
int getValidNumericInput(char *prompt, int *number);
int getValidFloatInput(char *prompt, float *number);

// MAIN FUNCTION
int main() {
    char username[20], password[20];
    char correctUser[] = "admin";
    char correctPass[] = "1234";
    int choice;
    char tryAgain;
    int loggedIn = 0;
    char input[100]; // Buffer for input validation

    printf("\n\t=====================================");
    printf("\n\t=                                   =");
    printf("\n\t=       COLLEGE OF TECHNOLOGIES     =");
    printf("\n\t=      Student Body Organization    =");
    printf("\n\t=          Logging System           =");
    printf("\n\t=                                   =");
    printf("\n\t=====================================\n");

    printf("\n\tPress Enter to continue...");
    getchar();

    system("cls");

    // LOGIN LOOP
    do {
        printf("\n\t\t------ LOG IN ------\n");

        // Username input with validation
        while (1) {
            printf("\n\tEnter Username: ");
            if (getValidInput("", username, 20)) {
                break;
            }
        }

        printf("\tEnter Password: ");
        maskPassword(password);
        printf("\n");

        if (strcmp(username, correctUser) == 0 && strcmp(password, correctPass) == 0) {
            printf("\n\tLogin Successful! Welcome, %s.\n", username);
            loggedIn = 1;
        } else {
            printf("\n\tIncorrect username or password.\n");

            // Try again choice with validation
            while (1) {
                printf("\tDo you want to try again? (y/n): ");
                if (getValidInput("", input, sizeof(input))) {
                    tryAgain = input[0];
                    if (tryAgain == 'y' || tryAgain == 'Y' || tryAgain == 'n' || tryAgain == 'N') {
                        break;
                    }
                    printf("\tPlease enter 'y' or 'n'.\n");
                }
            }
        }

    } while (!loggedIn && (tryAgain == 'y' || tryAgain == 'Y'));

    if (!loggedIn) {
        printf("\n\tExiting program. Goodbye!\n");
        return EXIT_SUCCESS;
    }

    printf("\n\tPress Enter to continue...");
    getchar();
    getchar();

    // MAIN MENU
    while (true) {
        system("cls");
        printf("\n\t===============================");
        printf("\n\t             MENU              ");
        printf("\n\t===============================");
        printf("\n\t1. Log as a Student");
        printf("\n\t2. Log Officer");
        printf("\n\t3. Payment");
        printf("\n\t4. View Log");
        printf("\n\t5. Exit");
        printf("\n\t===============================");
        printf("\n\tChoose (1-5): ");

        choice = getch();

        switch (choice) {
            case '1': {
                char studentID[20], firstName[30], lastName[30], fullName[61];
                char purpose[100];
                char subChoice, paymentChoice;
                FILE *logFile;
                time_t now;
                struct tm *local;
                char dateStr[20], timeStr[20];

                system("cls");
                printf("\n\t====== Student Log ======\n");

                // Input validation for Student ID
                while (1) {
                    printf("\tEnter Student ID: ");
                    if (getValidInput("", studentID, 20)) {
                        break;
                    }
                }

                // Input validation for First Name
                while (1) {
                    printf("\tEnter First Name: ");
                    if (getValidInput("", firstName, 30)) {
                        break;
                    }
                }

                // Input validation for Last Name
                while (1) {
                    printf("\tEnter Last Name: ");
                    if (getValidInput("", lastName, 30)) {
                        break;
                    }
                }

                snprintf(fullName, sizeof(fullName), "%s %s", firstName, lastName);

                while (1) {
                    system("cls");
                    printf("\n\t--- Student Log Options ---\n");
                    printf("\t1. Payment\n");
                    printf("\t2. Sizing\n");
                    printf("\t3. Other Purpose\n");

                    printf("\n\tChoose (1-3): ");
                    subChoice = getch();

                    time(&now);
                    local = localtime(&now);
                    // Separate date and time
                    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", local);
                    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", local);

                    logFile = fopen("student_log.txt", "a+");
                    if (logFile) {
                        // Check if file is empty
                        fseek(logFile, 0, SEEK_END);
                        long size = ftell(logFile);
                        if (size == 0) {
                            fprintf(logFile, "\t\t\t\t\tSTUDENT LOGS\n");
                            fprintf(logFile, "\t|--------------------------------------------------------------------------------------------------------------------|\n");
                            fprintf(logFile, "\t| %-12s | %-10s | %-15s | %-25s | %-35s |\n", "Date", "Time", "Student ID", "Name", "Purpose");
                            fprintf(logFile, "\t|--------------------------------------------------------------------------------------------------------------------|\n");
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
                                    // Input validation for amount
                                    while (1) {
                                        printf("\n\tEnter amount to pay: ");
                                        if (getValidFloatInput("", &amount)) {
                                            break;
                                        }
                                    }
                                    sprintf(purpose, "Payment: College Shirt (%.2f)", amount);
                                } else if (paymentChoice == '2') {
                                    // Input validation for payment purpose
                                    while (1) {
                                        printf("\n\tEnter Payment Purpose: ");
                                        if (getValidInput("", tempPurpose, 100)) {
                                            break;
                                        }
                                    }

                                    // Input validation for amount
                                    while (1) {
                                        printf("\tEnter amount to pay: ");
                                        if (getValidFloatInput("", &amount)) {
                                            break;
                                        }
                                    }

                                    sprintf(purpose, "Payment: %s (%.2f)", tempPurpose, amount);
                                } else {
                                    printf("\n\tInvalid payment option.\n");
                                    fclose(logFile);
                                    break;
                                }
                                break;
                            }

                            case '2': {
                                char sizeInput[10];
                                // Input validation for t-shirt size
                                while (1) {
                                    printf("\n\tEnter T-shirt Size (e.g., S, M, L, XL): ");
                                    if (getValidInput("", sizeInput, 10)) {
                                        break;
                                    }
                                }
                                char temp[100] = "Sizing: ";
                                strcat(temp, sizeInput);
                                strcpy(purpose, temp);
                                break;
                            }

                            case '3': {
                                char purposeInput[100];
                                // Input validation for other purpose
                                while (1) {
                                    printf("\n\tEnter Other Purpose: ");
                                    if (getValidInput("", purposeInput, 100)) {
                                        break;
                                    }
                                }
                                char temp2[100] = "Purpose: ";
                                strcat(temp2, purposeInput);
                                strcpy(purpose, temp2);
                                break;
                            }

                            default:
                                printf("\n\tInvalid option. Press any key to try again...");
                                fclose(logFile);
                                getch();
                                continue;
                        }

                        // Write to log file with consistent formatting and separate date/time
                        fprintf(logFile, "| %-12s | %-10s | %-15s | %-25s | %-35s |\n",
                                dateStr, timeStr, studentID, fullName, purpose);
                        fclose(logFile);
                        printf("\n\tStudent Logging success at %s %s\n", dateStr, timeStr);

                        char again;
                        // Input validation for "add another purpose"
                        while (1) {
                            printf("\n\tDo you want to add another purpose? (Y/N): ");
                            if (getValidInput("", input, sizeof(input))) {
                                again = input[0];
                                if (again == 'Y' || again == 'y' || again == 'N' || again == 'n') {
                                    break;
                                }
                                printf("\tPlease enter 'Y' or 'N'.\n");
                            }
                        }

                        if (again == 'N' || again == 'n') break;
                    } else {
                        printf("\n\tError: Could not open student_log.txt\n");
                        break;
                    }
                }

                break;
            }

            case '2': {
                char officerID[20], officerName[50];
                FILE *logFile;
                time_t now;
                struct tm *local;
                char dateStr[20], timeStr[20];

                system("cls");
                printf("\n\t====== Officer Log ======\n");

                // Input validation for Officer ID
                while (1) {
                    printf("\tEnter Officer Student ID: ");
                    if (getValidInput("", officerID, 20)) {
                        break;
                    }
                }

                // Input validation for Officer Name
                while (1) {
                    printf("\tEnter Officer Name: ");
                    if (getValidInput("", officerName, 50)) {
                        break;
                    }
                }

                logFile = fopen("officer_log.txt", "a+");
                if (logFile) {
                    time(&now);
                    local = localtime(&now);
                    // Separate date and time
                    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", local);
                    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", local);

                    // Check if file is empty
                    fseek(logFile, 0, SEEK_END);
                    long size = ftell(logFile);
                    if (size == 0) {
                        fprintf(logFile, "\t\t\t\tOFFICER LOGS\n\n");
                        fprintf(logFile, "\t|--------------------------------------------------------------------------------------|\n");
                        fprintf(logFile, "\t| %-12s | %-10s | %-28s | %-22s |\n", "Date", "Time", "Officer ID", "Name");
                        fprintf(logFile, "\t|--------------------------------------------------------------------------------------|\n");
                    }

                    fprintf(logFile, "| %-12s | %-10s | %-28s | %-22s |\n",
                            dateStr, timeStr, officerID, officerName);
                    fclose(logFile);

                    printf("\n\tOfficer Logging success at %s %s\n", dateStr, timeStr);
                } else {
                    printf("\n\tError: Could not open officer_log.txt\n");
                }

                break;
            }

            case '3':
                printf("\n\tPayment (Feature under construction)\n");
                break;

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
                    printf("\n\t====== VIEW LOGS ======\n");
                    printf("\t1. View Student Logs\n");
                    printf("\t2. View Officer Logs\n");
                    printf("\t3. Return to Main Menu\n");
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
                                // Input validation for view option
                                while (1) {
                                    printf("\n\tDo you want to view logs from a specific date? (Y/N): ");
                                    if (getValidInput("", input, sizeof(input))) {
                                        viewOption = input[0];
                                        if (viewOption == 'Y' || viewOption == 'y' || viewOption == 'N' || viewOption == 'n') {
                                            break;
                                        }
                                        printf("\tPlease enter 'Y' or 'N'.\n");
                                    }
                                }

                                if (viewOption == 'Y' || viewOption == 'y') {
                                    // Input validation for date
                                    while (1) {
                                        printf("\tEnter date (YYYY-MM-DD): ");
                                        if (getValidInput("", viewDate, 11)) {
                                            // Basic date format validation
                                            if (strlen(viewDate) == 10 && viewDate[4] == '-' && viewDate[7] == '-') {
                                                break;
                                            }
                                            printf("\tInvalid date format. Use YYYY-MM-DD.\n");
                                        }
                                    }

                                    // Reset file position after header
                                    fseek(logFile, 0, SEEK_SET);
                                    for (int i = 0; i < headerLines; i++) {
                                        fgets(line, sizeof(line), logFile);
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
                                        printf("%s", line);
                                        foundToday = 1;
                                    }
                                }

                                if (!foundToday) {
                                    printf("\tNo logs found for today (%s).\n", currentDate);
                                }

                                // Ask if user wants to view logs from another date
                                // Input validation for view option
                                while (1) {
                                    printf("\n\tDo you want to view logs from a specific date? (Y/N): ");
                                    if (getValidInput("", input, sizeof(input))) {
                                        viewOption = input[0];
                                        if (viewOption == 'Y' || viewOption == 'y' || viewOption == 'N' || viewOption == 'n') {
                                            break;
                                        }
                                        printf("\tPlease enter 'Y' or 'N'.\n");
                                    }
                                }

                                if (viewOption == 'Y' || viewOption == 'y') {
                                    // Input validation for date
                                    while (1) {
                                        printf("\tEnter date (YYYY-MM-DD): ");
                                        if (getValidInput("", viewDate, 11)) {
                                            // Basic date format validation
                                            if (strlen(viewDate) == 10 && viewDate[4] == '-' && viewDate[7] == '-') {
                                                break;
                                            }
                                            printf("\tInvalid date format. Use YYYY-MM-DD.\n");
                                        }
                                    }

                                    // Reset file position after header
                                    fseek(logFile, 0, SEEK_SET);
                                    for (int i = 0; i < headerLines; i++) {
                                        fgets(line, sizeof(line), logFile);
                                    }

                                    int foundSpecific = 0;

                                    while (fgets(line, sizeof(line), logFile)) {
                                        // Check if line contains the specified date
                                        if (strstr(line, viewDate)) {
                                            printf("\t%s", line);
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

// Input validation function for strings
int getValidInput(char *prompt, char *buffer, int bufferSize) {
    char *result;

    if (prompt[0] != '\0') {
        printf("%s", prompt);
    }

    fflush(stdin); // Clear input buffer
    result = fgets(buffer, bufferSize, stdin);

    // Check if input is empty (just Enter pressed)
    if (result == NULL || buffer[0] == '\n' || buffer[0] == '\0') {
        printf("Input cannot be empty. Try again.\n");
        return 0;
    }

    // Remove newline character
    buffer[strcspn(buffer, "\n")] = 0;

    // Check if input is empty after removing newline
    if (strlen(buffer) == 0) {
        printf("Input cannot be empty. Try again.\n");
        return 0;
    }

    return 1;
}

// Input validation function for integer choices
int getValidChoice(char *prompt, char validChoices[], int numChoices) {
    char input[10];
    char choice;

    while (1) {
        if (!getValidInput(prompt, input, sizeof(input))) {
            continue;
        }

        if (strlen(input) != 1) {
            printf("Please enter a single character choice.\n");
            continue;
        }

        choice = input[0];
        for (int i = 0; i < numChoices; i++) {
            if (choice == validChoices[i]) {
                return choice;
            }
        }

        printf("Invalid choice. Try again.\n");
    }
}

// Input validation function for integers
int getValidNumericInput(char *prompt, int *number) {
    char input[100];

    if (!getValidInput(prompt, input, sizeof(input))) {
        return 0;
    }

    if (sscanf(input, "%d", number) != 1) {
        printf("Invalid number. Try again.\n");
        return 0;
    }

    return 1;
}

// Input validation function for floats
int getValidFloatInput(char *prompt, float *number) {
    char input[100];

    if (!getValidInput(prompt, input, sizeof(input))) {
        return 0;
    }

    if (sscanf(input, "%f", number) != 1) {
        printf("Invalid number. Try again.\n");
        return 0;
    }

    return 1;
}
