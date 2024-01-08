/* 
* Functions to save list / import existing lists
* Author: Tate Kolton
* Date Created: January 5, 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "../include/todo.h"

// Check if a given filename has a ".csv" extension
int isCSVFile(const char *filename) {
    const char *dot = strrchr(filename, '.');
    return dot && !strcmp(dot, ".csv");
}

// Get a list of CSV files in the current directory and process each filename
int getLists() {
    DIR *dir;
    struct dirent *entry;
    int emptyFlag = TRUE;

    // Open the current directory
    dir = opendir(".");
    if (dir == NULL) {
        perror("Error opening directory");
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (isCSVFile(entry->d_name)) {
            // Process each CSV filename
            processCSVFileName(entry->d_name);
            emptyFlag = FALSE;
        }
    }

    // Return 1 if any CSV files were found
    return !emptyFlag;
}

// Process a CSV filename to extract the portion before the ".csv" extension
void processCSVFileName(const char *filename) {
    // Find the position of the last dot ('.') in the filename
    const char *dot = strrchr(filename, '.');
    
    if (dot != NULL) {
        // Calculate the length of the portion before the dot
        size_t length = dot - filename;

        // Allocate memory for the portion before the dot
        char *portionBeforeDot = malloc(length + 1);

        // Copy the portion before the dot
        strncpy(portionBeforeDot, filename, length);
        portionBeforeDot[length] = '\0';  // Null-terminate the string

        // Print the portion before the dot
        printf("%s\n", portionBeforeDot);

        // Free the allocated memory
        free(portionBeforeDot);
    } 
}

// Find and open a file with the given filename in read mode
FILE *findList(char *filename) {
    // Allocate memory for the new filename
    char *list = malloc(strlen(filename) + strlen(".csv") + 1);


    // Check if memory allocation was successful
    if (list == NULL) {
        perror("Memory allocation error");
        return NULL;
    }

    // Add ".csv" file ending to the filename
    // Construct the filename with ".csv" extension
    sprintf(list, "%s.csv", filename);

    // Open the file for reading
    FILE *file = fopen(list, "r");
    
    // Check if file opening was successful
    if (file == NULL) {
        free(list); // Free memory in case of an error
        return NULL;
    }

    // Free memory associated with list
    free(list);

    // Return the file pointer
    return file;
}

// Import a list from a CSV file and populate the todo_list
List *importList(char *filename) {
    FILE *list_file;
    List *todo_list;

    printf("Trying to open file: %s\n", filename);


    // Find and open the specified list file
    if ((list_file = findList(filename)) == NULL) {
        printf("Couldn't open that list! Ensure list name is correct and it exists\n");
        free(filename);
        return NULL;
    } else {
        printf("List opened successfully!\n");

        // Create a new todo_list object
        if ((todo_list = createList(filename)) == NULL) {
            perror("Memory allocation error");
            return NULL;    
        } else {
            // Populate the todo_list with tasks from the CSV file
            populateList(todo_list, list_file);
        }
    }

    return todo_list;
}

// Populate a todo_list with tasks from a CSV file
void populateList(List *todo, FILE *file) {
    char line[MAX_LINE];

    // Read and parse each line in the file
    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove newline character from the end of the line
        line[strcspn(line, "\n")] = '\0';

        // Create a Task structure and allocate memory for it
        Todo *task = malloc(sizeof(Todo));
        if (task == NULL) {
            perror("Memory allocation error");
            return;
        }

        // Allocate memory for the description field
        task->description = malloc(MAX_DESCRIPTION); // Adjust the size as needed
        if (task->description == NULL) {
            perror("Memory allocation error");
            free(task); // Free allocated memory before returning
            return;
        }

        // Initialize the next field in task to none
        task->next = NULL;

        // Use sscanf to parse the line
        if (sscanf(line, "%249[^,], %d, %d",
                task->description, &task->date, &task->complete) == 3) {

            // Add task to the todo_list
            insertTask(task, todo);
        } else {
            // Handle parsing error
            printf("Error parsing line: %s\n", line);
            free(task->description);
            free(task);
        }
    }
}

// Open up a saved list in CSV format and allocate memory for it
List *openList() {

    List *todo_list;
    char *name = malloc(MAX_DESCRIPTION);

    if(name == NULL) {
        perror("Memory allocation error");
        return NULL;
    }

    printf("\nSaved Lists: \n");
    printf("____________\n\n");
    
    if(!getLists()) {
        printf("No existing lists available\n");
        free(name);
        return NULL;
    }

    printf("\nSelect List: ");
    fgets(name, MAX_DESCRIPTION + 1, stdin);

    // Remove newline character from input
    size_t len = strlen(name);
    if (len > 0 && name[len - 1] == '\n') {
        name[len - 1] = '\0';
    }

    // Open user entered list
    todo_list = importList(name);

    return todo_list;
}

int saveList(List *list) {

    if (list == NULL || list->listName == NULL) {
        return 0;
    }

    char *filename = malloc(strlen(list->listName) + strlen(".csv") + 1);

    if (filename == NULL) {
        perror("Memory allocation error");
        return 0;
    }

    // Construct the filename with ".csv" extension
    sprintf(filename, "%s.csv", list->listName);

    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        perror("Error opening file for writing");
        free(filename);
        return 0;
    }

    // Write tasks to the CSV file
    Todo *currentTask = list->head;
    while (currentTask != NULL) {
        fprintf(file, "%s, %d, %d\n", currentTask->description, currentTask->date, currentTask->complete);
        currentTask = currentTask->next;
    }

    fclose(file);
    free(filename);

    return 1;  // Return 1 for success
}
