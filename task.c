/* 
* C Program to create and organize a To-Do List 
* Author: Tate Kolton
* Date Created: January 2, 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include "todo.h"

Todo *getTask() {

    /* Flush the input buffer */
    //fflush(stdin);

    char *description = getDescription();
    int date = getDate();

    Todo *task = (Todo*)malloc(sizeof(Todo));

    task->description = description;
    task->date = date;
    task->complete = FALSE;
    task->next = NULL;

    return task;
}

char *getDescription() {
    char *desc = (char*)malloc(MAX_DESCRIPTION * sizeof(char));
    char c;
    int i;

    /* Prompt user for input */
    printf("\nTask Description: ");

    /* Read task */
    for (i = 0; i < MAX_DESCRIPTION && (c = getchar()) != '\n' && c != EOF; i++)
        desc[i] = (char)c;

    /* Add null char */
    desc[i] = '\0';

    /* Resize the string based on the actual length */
    desc = realloc(desc, i + 1);

    return desc;
}


int getDate() {
    int days_until_due;

    /* Prompt user for input */
    printf("Number of Days Until Task is Due: ");

    /* Read days until the task is due with input validation */
    while (scanf("%d", &days_until_due) != 1) {
        printf("\nInvalid input. Please enter a valid integer: ");
    }

    /* Clear newline from input buffer */
    char c = getchar();

    return days_until_due;
}


void freeTask(Todo *task) {

    if (task != NULL) {
        free(task->description);
        free(task);
    }
}



