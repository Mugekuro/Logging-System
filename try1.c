#include <stdio.h>
#include <string.h>


int main() {
    char username[20], password[20];
    char correctUser[] = "admin";
    char correctPass[] = "1234";
    int choice;



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
    scanf("%s", password);

    // Check if login credentials are correct
    if (strcmp(username, correctUser) == 0 && strcmp(password, correctPass) == 0) {
        // Display login success message first
        printf("\n\tLogin Successful! Welcome, %s.\n", username);

        // Add a short delay before clearing the screen
        printf("\n\tPress Enter to continue...");
        getchar();  // Consume newline left by previous scanf
        getchar();  // Wait for Enter key press

        while (1) {
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
            printf("\n\tEnter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    printf("\n\tYou chose: Log as a Student\n");
                    break;
                case 2:
                    printf("\n\tYou chose: Log Officer\n");
                    break;
                case 3:
                    printf("\n\tYou chose: Payment\n");
                    break;
                case 4:
                    printf("\n\tYou chose: View Log\n");
                    break;
                case 5:
                    printf("\n\tExiting... Goodbye!\n");
                    return 0;
                default:
                    printf("\n\tInvalid choice. Try again.\n");
            }
        }
    } else {
        printf("\n\tInvalid Username or Password. Try again.\n");
    }

    return 0;
}
