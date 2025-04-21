#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> //For true/false
#include <conio.h> // For getch()





//FUNCTION PROTOTYPE
void maskPassword(char password[]);


//MAIN FUNCTION ENTRY

int main() {
    char username[20], password[20];
    char correctUser[] = "admin";
    char correctPass[] = "1234";
    int choice;

    //comment

    printf("\n\t=====================================");
    printf("\n\t=                                   =");
    printf("\n\t=       COLLEGE OF TECHNOLOGIES     =");
    printf("\n\t=      Student Body Organization    =");
    printf("\n\t=          Logging System           =");
    printf("\n\t=                                   =");
    printf("\n\t=====================================\n");

            // Add a short delay before clearing the screen
        printf("\n\tPress Enter to continue...");
        getchar();  // Consume newline left by previous scanf



    system("cls");

    printf("\n\t\t------ LOG IN ------\n");

    // Get username and password
    printf("\n\tEnter Username: ");
    scanf("%s", username);

    printf("\tEnter Password: ");
    maskPassword(password);
    printf("\n");

    // Check if login credentials are correct
    if (strcmp(username, correctUser) == 0 && strcmp(password, correctPass) == 0) {
        // Display login success message first
        printf("\n\tLogin Successful! Welcome, %s.\n", username);

        // Add a short delay before clearing the screen
        printf("\n\tPress Enter to continue...");
        getchar();  // Consume newline left by previous scanf
        getchar();  // Wait for Enter key press

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


            choice = getch(); //https://markaicode.com/learn-getch-function-in-c-complete-guide-with-examples/
            switch (choice) {
                case '1':
                    printf("\n\tYou chose: Log as a Student\n");
                    break;
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

                    logFile = fopen("officer_log.txt", "a");
                    if (logFile == NULL) {
                        printf("\n\tError: Could not open officer_log.txt\n");
                    } else {
                        time(&now);
                        local = localtime(&now);
                        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", local);

                        fprintf(logFile, "Officer ID: %s | Name: %s | Logged at: %s\n", officerID, officerName, timeStr);
                        fclose(logFile);

                        printf("\n\tOfficer Logging success at %s\n", timeStr);
                    }

                    break;
                }

                case '3':
                    printf("\n\tYou chose: Payment\n");
                    break;
                case '4': {
                    char subChoice;
                    FILE *logFile;
                    char line[256];

                    system("cls");
                    printf("\n\t====== VIEW LOGS ======\n");
                    printf("\t1. View Student Logs\n");
                    printf("\t2. View Officer Logs\n");
                    printf("\n\tChoose (1-2): ");
                    subChoice = getch();  // Get input without pressing enter
                    printf("%c\n", subChoice); // Echo choice

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
                        default:
                            printf("\n\tInvalid choice.\n");
                    }

                    break;
                }

                case '5':
                    printf("\n\tExiting... Goodbye!\n");
                    return EXIT_SUCCESS;
                default:
                    printf("\n\tInvalid choice. Try again.\n");
            }

            // Add a short delay before showing the menu again
            printf("\n\tPress any key to continue...");
            getch();
        }
    } else {
        printf("\n\tInvalid Username or Password. Try again.\n");
    }

    return EXIT_SUCCESS;
}


//FUNCTION DEFINITION
void maskPassword(char password[]) { //https://www.youtube.com/watch?v=-lexb9VFCPM
    int i = 0;
    char ch;
    while ((ch = _getch()) != 13) {
        printf("*");
        password[i] = ch;
        i++;
    }
    password[i] = '\0';
}
