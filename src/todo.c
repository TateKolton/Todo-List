#include <stdio.h>
#include <stdlib.h>
#include "../include/todo.h"


int main() {

    List *taskList;

    printf("\n**  TO-DO LIST SOFTWARE APPLICATION  **\n********  AUTHOR: TATE KOLTON  ********\n\n");

    // Prompt user for command
    printf("\nEnter a command ('newList' to create a new To-Do list, 'openList' to open an existing To-Do List, 'exit' to quit): ");

    while(1) {
        char input[50];
        fgets(input, sizeof(input), stdin);

        // Remove newline character from input
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }

        if (strcmp(input, "newList") == 0) {
            taskList = createNewList(); break;
        } else if(strcmp(input, "openList") == 0) {
            taskList = openList();
            if(taskList == NULL) {
                return 0;
            }
            break;
        } else if(strcmp(input, "exit") == 0) {
            return 0;
        } else { 
                printf("Unknown command. Try again.\n");
        }
    }
    
    while (1) { // Infinite loop to keep the program running
        char input[50];

        printf("\nEnter a command ('addTask' to add a task, 'printList' to print the list, 'exit' to quit): ");
        fgets(input, sizeof(input), stdin);

        // Remove newline character from input
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }

        if (strcmp(input, "addTask") == 0) {
            Todo *newTask = getTask();
            if (insertTask(newTask, taskList)) {
                printf("Task successfully added to list!\n");
            } else {
                printf("ERROR: Task could not be added to list!\n");
            }
        } else if (strcmp(input, "printList") == 0) {
            printList(taskList);
        } else if (strcmp(input, "exit") == 0) {
            break; // Exit the infinite loop
        } else { 
            printf("Unknown command. Try again.\n");
        }
    }

    if(saveList(taskList))
        printf("List saved successfully as: %s.csv\n", taskList->listName);
    else
        printf("Couldn't save list!");

    // Clean up and free memory before exiting
    deleteList(taskList);

    return 0;
}

List *createList(char *name) {

    List *todo_list = (List *)malloc(sizeof(List));
    
    if (todo_list == NULL) {
        fprintf(stderr, "Memory allocation error for List\n");
        exit(EXIT_FAILURE);
    }

    /* Assign a name to the list */
    todo_list->listName = name;

    /* Fill in first task in list */
    todo_list->head = NULL;

    return todo_list;
}

List *createNewList() {
    /* Prompt user for input */
    printf("Enter a name for the To-Do list: ");
    int i;
    char *name = malloc(MAX_DESCRIPTION);

    if (name == NULL) {
    fprintf(stderr, "Memory allocation error for name\n");
    exit(EXIT_FAILURE);
    }

    /* Read task */
    for (i = 0; i < MAX_DESCRIPTION && (name[i] = getchar()) != '\n' && name[i] != EOF; i++)
        ;

    /* Add null char */
    name[i] = '\0';

    List *taskList = createList(name);

    return taskList;
}

int insertTask(Todo *toInsert, List *todo_list) {
    if (toInsert == NULL) {
        return 0; // Handle NULL task
    }

    if (todo_list->head == NULL || toInsert->date < todo_list->head->date) {
        toInsert->next = todo_list->head;
        todo_list->head = toInsert;
        return 1; // Task successfully added as new head of list
    }

    Todo *ptr = todo_list->head;
    Todo *temp;

    while (ptr->next != NULL && ptr->next->date < toInsert->date) {
        ptr = ptr->next;
    }

    temp = ptr->next;
    ptr->next = toInsert;
    toInsert->next = temp;

    return 1; // Task successfully added
}


int deleteList(List *list) {
    if (list != NULL) {
        Todo *ptr = list->head;

        while (ptr != NULL) {
            Todo *temp = ptr;
            ptr = ptr->next;
            freeTask(temp);
        }

        free(list->listName); // Free the memory for listName
        free(list);

        return 1;
    }

    return 0;
}

void printList(List *list) {

    Todo *current = list->head;

    if (current == NULL) {
        printf("Task list is empty.\n");
    } else {
        printf("\n%s:\n", list->listName);
        printf("____________________________\n\n");
        while (current != NULL) {
            printf("Description: %-50s Days Until Due: %d\n", current->description, current->date);
            current = current->next;
        }
    }
}