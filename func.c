#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "file.h"

char todo[100];
struct Todo {
     int actualID;
    char description[100];
    char endDate[20];
    int isCompleted;
};
void menu() {
    system("color 0F");
    int choice;
start:
    system("cls");
    printf("\t\t\t\t\t<<------- ToDo LIST ------->>\n\n");
    system("color 0F");
    printf("\t\t\t\t\t-------Instructions:\n");
    printf("\t\t\t\t\t----------\n");
    printf("\t\t\t\t\t 1. Add ToDo\n");
    printf("\t\t\t\t\t 2. View ToDo\n");
    printf("\t\t\t\t\t 3. Delete ToDo\n");
    printf("\t\t\t\t\t 4. Modify ToDo\n");
    printf("\t\t\t\t\t 5. Reset ToDo\n");
    printf("\t\t\t\t\t 6. EXIT\n");
    system("color 0F");
    printf("\t\t\t\t\t \nChoose : ");
    fflush(stdin);
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            addToDoS();
            break;
        case 2:
            viewToDoS();
            break;
        case 3:
            deleteToDoS();
            break;
        case 4:
            modifyToDo();
            break;
        case 5:
           resetToDoS();
        case 6:
            exit(0);
        default:
            printf("\nInvalid Command\n");
            printf("Press any key to retry ");
            fflush(stdin);
            getch();
            goto start;
    }
}

//FUNC TO CHECK TIME AND DATE
int isValidDateTime(const char *dateTime) {
    // Date and time format: YYYY-MM-DD HH:MM
    int year, month, day, hour, minute;
    if (sscanf(dateTime, "%d-%d-%d %d:%d", &year, &month, &day, &hour, &minute) != 5) {
        return 0; // Not enough or too many fields
    }

    if (year < 0 || month < 1 || month > 12 || day < 1 || hour < 0 || hour > 23 || minute < 0 || minute > 59) {
        return 0; // Invalid year, month, day, hour, or minute
    }

    // Check for valid days in each month
    if (day > 31 ||
        (month == 4 || month == 6 || month == 9 || month == 11) && day > 30 ||
        (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) ? day > 29 : day > 28)) {
        return 0;
    }

    return 1; // Valid date and time format
}

//FUNC TO Compare DATES AND TIME FOR QUICKSORT
int compareDateTime(const void *a, const void *b) {
    struct Todo *todoA = (struct Todo *)a;
    struct Todo *todoB = (struct Todo *)b;
    return strcmp(todoA->endDate, todoB->endDate);
}

//FUNC TO DISPLAY
void viewToDoS() {
    system("color 0F");
    FILE *file;
    system("cls");
    struct Todo todos[100];
    printf("\t\tYour ToDo List\n");
    printf("\t\t--------------\n\n");
    file = fopen("todo.txt", "rb+");
    if (file == NULL) {
        printf("You don't have any ToDos yet\n");
        printf("\nPress any key to continue ");
        fflush(stdin);
        getch();
        menu();
    }
    int numTodos = 0;
    while (fread(&todos[numTodos], sizeof(struct Todo), 1, file)) {
        numTodos++;
    }
    fclose(file);

    if (numTodos == 0) {
        printf("You don't have any ToDos yet\n");
        printf("\nPress any key to continue ");
        fflush(stdin);
        getch();
        menu();
    }

    // Sort the todos based on end date and time
    qsort(todos, numTodos, sizeof(struct Todo), compareDateTime);

    // Display the sorted list
    for (int i = 0; i < numTodos; i++) {
        printf("%d. Description: %s", i + 1, todos[i].description);
        printf("   End Date: %s   ", todos[i].endDate);
        printf("Completed: %s\n", (todos[i].isCompleted == 1) ? "Yes" : "No");
    }

    printf("\nPress any key to continue ");
    fflush(stdin);
    getch();
    menu();
}

//FUNC TO ADD TASKS
void addToDoS() {
    system("color 0A");
    FILE *file;
    system("cls");
    char add = 'y';
    struct Todo todo;
    printf("\t\tAdd ToDo\n");
    printf("\t\t--------\n\n");

    file = fopen("todo.txt", "ab+");

    while (add == 'y' || add == 'Y') {
        printf("Add a todo: ");
        fflush(stdin);
        fgets(todo.description, sizeof(todo.description), stdin);

        printf("Enter the ending date and time (YYYY-MM-DD HH:MM): ");
        fgets(todo.endDate, sizeof(todo.endDate), stdin);
        size_t len = strlen(todo.endDate);
        if (len > 0 && todo.endDate[len - 1] == '\n') {
            todo.endDate[len - 1] = '\0'; // Remove newline character
        }

        // Validate the date and time
        if (!isValidDateTime(todo.endDate)) {
            printf("Invalid date and time format. Please use YYYY-MM-DD HH:MM.\n");
            continue; // Ask for date and time again
        }

        // Prompt for completion status with error handling
        int isValidInput = 0;
        do {
            printf("Is the task completed? (1 for completed, 0 for not completed): ");
            fflush(stdin);
            if (scanf("%d", &todo.isCompleted) != 1 || (todo.isCompleted != 0 && todo.isCompleted != 1)) {
                printf("Invalid input. Please enter 0 for not completed or 1 for completed.\n");
            } else {
                isValidInput = 1;
            }
        } while (!isValidInput);

        fwrite(&todo, sizeof(todo), 1, file);

        printf("\nWant to add another? (y/n): ");
        add = getch();
        printf("\n\n");
    }

    fclose(file);
    menu();
}

//FUNC TO DELETE TASKS
void deleteToDoS() {
    system("color FC");
    FILE *file, *fite;
    system("cls");
    int indexToDelete;
    printf("\t\tDelete ToDo\n");
    printf("\t\t-----------\n\n");

    // Display the list of tasks with their IDs
    printf("List of Tasks:\n");
    printf("ID\tDescription\n");
    printf("--\t-----------\n");

    file = fopen("todo.txt", "rb+");
    struct Todo todo;
    int numTodos = 0;

    // Read and display tasks from the file
    while (fread(&todo, sizeof(struct Todo), 1, file)) {
        printf("%d\t%s\n", numTodos + 1, todo.description);
        numTodos++;
    }

    fclose(file);

    // Ask the user for the ID of the task to delete
    printf("\nEnter the ID of the ToDo you want to delete: ");
    scanf("%d", &indexToDelete);

    // Open files for reading and writing
    file = fopen("todo.txt", "rb+");
    fite = fopen("temp.txt", "wb+");

    // Rewrite tasks to a temporary file, excluding the one to delete
    for (int i = 0; i < numTodos; i++) {
        // Skip writing the task with the specified ID
        if (i + 1 != indexToDelete) {
            fread(&todo, sizeof(struct Todo), 1, file);
            fwrite(&todo, sizeof(struct Todo), 1, fite);
        } else {
            // Read and discard the task to delete
            fread(&todo, sizeof(struct Todo), 1, file);
        }
    }

    fclose(file);
    fclose(fite);

    // Replace the original file with the temporary file
    remove("todo.txt");
    rename("temp.txt", "todo.txt");

    printf("\nTask with ID %d deleted.\n", indexToDelete);
    printf("\nPress any key to continue");
    getch();
    menu();
}

void modifyToDo() {
    system("color 0E");
    FILE *file;
    system("cls");
    int indexToModify;
    printf("\t\tModify ToDo\n");
    printf("\t\t-----------\n\n");

    // Display the list of tasks with their IDs
    printf("List of Tasks:\n");
    printf("ID\tDescription\n");
    printf("--\t-----------\n");

    file = fopen("todo.txt", "rb+");
    struct Todo todo;
    int found = 0;
    int numTodos = 0;

    // Read and display tasks from the file
    while (fread(&todo, sizeof(struct Todo), 1, file)) {
        numTodos++;
        printf("%d\t%s\n", numTodos, todo.description);
    }

    // Ask the user for the position of the task to modify
    printf("\nEnter the ID of the ToDo you want to modify: ");
    scanf("%d", &indexToModify);

    // Validate user input
    if (indexToModify < 1 || indexToModify > numTodos) {
        printf("Invalid ID. No ToDo modified.\n");
        fclose(file);
        printf("\nPress any key to continue");
        getch();
        menu();
        return;
    }

    // Move the file pointer to the position of the task to modify
    fseek(file, (indexToModify - 1) * sizeof(struct Todo), SEEK_SET);

    // Read the task to modify
    fread(&todo, sizeof(struct Todo), 1, file);

    // Modify the task description
    printf("Current Description: %s\n", todo.description);
    printf("Enter new Description: ");
    fflush(stdin);
    fgets(todo.description, sizeof(todo.description), stdin);

    // Modify the end date
    printf("Current End Date: %s\n", todo.endDate);
    printf("Enter new End Date (YYYY-MM-DD HH:MM): ");
    fflush(stdin);
    fgets(todo.endDate, sizeof(todo.endDate), stdin);
    size_t len = strlen(todo.endDate);
    if (len > 0 && todo.endDate[len - 1] == '\n') {
        todo.endDate[len - 1] = '\0'; // Remove newline character
    }

    // Validate the date and time
    while (!isValidDateTime(todo.endDate)) {
        printf("Invalid date and time format. Please use YYYY-MM-DD HH:MM.\n");
        printf("Enter new End Date (YYYY-MM-DD HH:MM): ");
        fflush(stdin);
        fgets(todo.endDate, sizeof(todo.endDate), stdin);
        size_t len = strlen(todo.endDate);
        if (len > 0 && todo.endDate[len - 1] == '\n') {
            todo.endDate[len - 1] = '\0'; // Remove newline character
        }
    }

    // Modify the state
    printf("Current State: %d\n", todo.isCompleted);
    printf("Enter new State (1 for Completed, 0 for Not Completed): ");
    int newState;
    while (scanf("%d", &newState) != 1 || (newState != 0 && newState != 1)) {
        printf("Invalid input. Please enter 0 for not completed or 1 for completed.\n");
        printf("Enter new State (1 for Completed, 0 for Not Completed): ");
        fflush(stdin);
    }
    todo.isCompleted = newState;

    // Move the file pointer back to the beginning of the task
    fseek(file, (indexToModify - 1) * sizeof(struct Todo), SEEK_SET);

    // Write the modified task back to the file
    fwrite(&todo, sizeof(struct Todo), 1, file);

    fclose(file);
    printf("\nToDo modified successfully.\n");
    printf("\nPress any key to continue");
    getch();
    menu();
}


//FUNC TO RESET ALL TASKS
void resetToDoS(){
    system("color CF");
    FILE *file, *fite1;
    system("cls");
    printf("\t\tReset ToDoS\n");
    printf("\t\t-----------\n\n");
    printf("Do you want to reset your ToDoS list ?(y/n)");
    fflush(stdin);
    char a = getch();
    if (a == 'y' || a == 'Y')
    {
    }
    else
    {
        printf("\n\nPress any key to continue ");
        fflush(stdin);
        getch();
        menu();
    }

    file = fopen("todo.txt", "rb+");
    fite1 = fopen("temp1.txt", "wb+");

    while (fread(&todo, sizeof(todo), 1, file))
    {
        fwrite(&todo, sizeof(fite1), 1, fite1);
    }

    printf("\n\nYour ToDoS list has been reset\n");
    fclose(file);
    fclose(fite1);
    remove("todo.txt");
    rename("temp1.txt", "todo.txt");
    fflush(stdin);
    printf("\nPress any key to continue");
    getch();
    menu();
}
