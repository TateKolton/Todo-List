/* Header file for To Do List Program */
#include <string.h>

#define TRUE 1
#define FALSE 0
#define MAX_DESCRIPTION 200


typedef struct Todo {
    char *description;
    int complete;
    int date;
    struct Todo *next;
} Todo;

typedef struct List {
    Todo *head;
    char *listName;
} List;

/* Task Functions */
Todo *getTask();
char *getDescription();
int getDate();
void freeTask(Todo *task);

/* List Functions */
List *createList();
int insertTask(Todo* toInsert, List* todo_list);
int deleteList(List *list);
void printList(List *list);