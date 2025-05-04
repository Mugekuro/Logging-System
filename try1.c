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
        printf("\n\tEnter Username: ");
        scanf("%s", username);

        printf("\tEnter Password: ");
        maskPassword(password);
        printf("\n");

        if (strcmp(username, correctUser) == 0 && strcmp(password, correctPass) == 0) {
            printf("\n\tLogin Successful! Welcome, %s.\n", username);
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
                char timeStr[100];

                system("cls");
                printf("\n\t====== Student Log ======\n");

                printf("\tEnter Student ID: ");
                scanf("%s", studentID);

                printf("\tEnter First Name: ");
                scanf("%s", firstName);

                printf("\tEnter Last Name: ");
                scanf("%s", lastName);

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
                    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", local);

                    logFile = fopen("student_log.txt", "a+");
                    if (logFile) {
                        // Check if file is empty
                        fseek(logFile, 0, SEEK_END);
                        long size = ftell(logFile);
                        if (size == 0) {
                            fprintf(logFile, "\t\t\t\t\tSTUDENT LOGS\n");
                            fprintf(logFile, "|------------------------------------------------------------------------------------------------|\n");
                            fprintf(logFile, "| %-20s | %-15s | %-25s | %-25s |\n", "Date & Time", "Student ID", "Name", "Purpose");
                            fprintf(logFile, "|------------------------------------------------------------------------------------------------|\n");
                        }

                        switch (subChoice) {
                            case '1':
                                printf("\n\t--- Payment Options ---\n");
                                printf("\t1. College Shirt\n");
                                printf("\t2. Other\n");
                                printf("\n\tChoose (1-2): ");
                                paymentChoice = getch();

                                if (paymentChoice == '1') {
                                    strcpy(purpose, "Payment: College Shirt");
                                } else if (paymentChoice == '2') {
                                    printf("\n\tEnter Payment Purpose: ");
                                    scanf(" %[^\n]", purpose);
                                    char temp[100] = "Payment: ";
                                    strcat(temp, purpose);
                                    strcpy(purpose, temp);
                                } else {
                                    printf("\n\tInvalid payment option.\n");
                                    fclose(logFile);
                                    break;
                                }
                                break;

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

                            default:
                                printf("\n\tInvalid option. Press any key to try again...");
                                fclose(logFile);
                                getch();
                                continue;
                        }

                        // Write to log file with consistent formatting
                        fprintf(logFile, "| %-20s | %-15s | %-25s | %-25s |\n",
                                timeStr, studentID, fullName, purpose);
                        fclose(logFile);
                        printf("\n\tStudent Logging success at %s\n", timeStr);

                        char again;
                        printf("\n\tDo you want to add another purpose? (Y/N): ");
                        scanf(" %c", &again);

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
                char timeStr[100];

                system("cls");
                printf("\n\t====== Officer Log ======\n");

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
                        fprintf(logFile, "\t\t\t\tOFFICER LOGS\n");
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

            case '3':
                printf("\n\tPayment (Feature under construction)\n");
                break;

            case '4': {
                char subChoice;
                FILE *logFile;
                char line[256];

                while (1) {
                    system("cls");
                    printf("\n\t====== VIEW LOGS ======\n");
                    printf("\t1. View Student Logs\n");
                    printf("\t2. View Officer Logs\n");
                    printf("\t3. Return to Main Menu\n");
                    printf("\n\tChoose (1-3): ");
                    subChoice = getch();

                    switch (subChoice) {
                        case '1':
                            logFile = fopen("student_log.txt", "r");
                            if (logFile == NULL) {
                                printf("\n\tNo student logs found.\n");
                            } else {
                                printf("\n\t--- Student Logs ---\n");
                                while (fgets(line, sizeof(line), logFile)) {
                                    printf("\t%s", line);
                                }
                                fclose(logFile);
                            }
                            break;

                        case '2':
                            logFile = fopen("officer_log.txt", "r");
                            if (logFile == NULL) {
                                printf("\n\tNo officer logs found.\n");
                            } else {
                                printf("\n\t--- Officer Logs ---\n");
                                while (fgets(line, sizeof(line), logFile)) {
                                    printf("\t%s", line);
                                }
                                fclose(logFile);
                            }
                            break;

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
