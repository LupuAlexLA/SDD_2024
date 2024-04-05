#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINESIZE 128

typedef struct Task Task;
typedef struct Node Node;
typedef struct DLList DLList;

struct Task
{
    int taskId;
    char* description;
    float resourceUtilization;
};

struct Node
{
    Task task;
    Node* next;
    Node* prev;
};

struct DLList
{
    Node* head;
    Node* tail;
};

void printTaskToConsole(Task task)
{
    printf("Task %d:\n\tDescription: %s\tResource utilization: %.2f%%\n",
           task.taskId, task.description, task.resourceUtilization);
}

Task readTaskFromFile(FILE* f)
{
    char buffer[LINESIZE];

    Task task;

    fgets(buffer, LINESIZE, f);
    task.taskId = (int)strtol(buffer, NULL, 10);

    fgets(buffer, LINESIZE, f);
    task.description = malloc((strlen(buffer) + 1));
    strcpy(task.description, buffer);

    fgets(buffer, LINESIZE, f);
    task.resourceUtilization = strtof(buffer, NULL);

    return task;
}

void readTasksFromFile(const char* filename, Task** tasksArray, int* tasksNumber)
{
    FILE* f = fopen(filename, "r");

    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    *tasksNumber = (int)strtol(buffer, NULL, 10);

    *tasksArray = malloc((*tasksNumber) * sizeof(Task));

    for (int i = 0; i < *tasksNumber; i++)
    {
        (*tasksArray)[i] = readTaskFromFile(f);
    }

    fclose(f);
}

Task deepCopyTask(Task task)
{
    Task copyTask;
    copyTask.taskId = task.taskId;
    copyTask.description = malloc((strlen(task.description) + 1));
    strcpy(copyTask.description, task.description);
    copyTask.resourceUtilization = task.resourceUtilization;
    return copyTask;
}

void insertAtBeginning(DLList* dll, Task task)
{
    Node* newNode = malloc(sizeof(Node));
    newNode->task = task;

    newNode->next = (*dll).head; // dll->next <=> (*dll).head
    newNode->prev = NULL;

    if ((*dll).head)
    {
        (*dll).head->prev = newNode; // era null
    }
    else
    {
        (*dll).tail = newNode;
    }

    (*dll).head = newNode; // newNode in cazul asta e si head si tail
}

void insertAtEnd(DLList* dll, Task task)
{
    Node* newNode = malloc(sizeof(Node));
    newNode->task = task;

    newNode->next = NULL;
    newNode->prev = dll->tail; //(*dll).tail - daca vreau sa ii modific valoarea, daca nu fac asta, in main nu o sa se reflecte

    if ((*dll).tail)
    {
        (*dll).tail->next = newNode; // era null
    }
    else
    {
        (*dll).head = newNode;
    }

    (*dll).tail = newNode;
}

void parseListBeginningToEnd(DLList dll)
{
    if (dll.head)
    {
        while (dll.head)
        {
            printTaskToConsole(dll.head->task);
            dll.head = dll.head->next;
        }
    }
    else
    {
        printf("List is empty!");
    }
}

void parseListEndToBeginning(DLList dll)
{
    if (dll.tail)
    {
        while (dll.tail)
        {
            printTaskToConsole(dll.tail->task);
            dll.tail = dll.tail->prev;
        }
    }
    else
    {
        printf("List is empty!");
    }
}

int main()
{
    int tasksNumber;
    Task* tasksArray;

    readTasksFromFile("scheduler.txt", &tasksArray, &tasksNumber);

    for (int i = 0; i < tasksNumber; i++)
    {
        printTaskToConsole(tasksArray[i]);
    }

    printf("---------------------\n");

    DLList dll;
    dll.head = NULL;
    dll.tail = NULL;

    insertAtBeginning(&dll, deepCopyTask(tasksArray[0]));
    insertAtBeginning(&dll, deepCopyTask(tasksArray[1]));
    insertAtBeginning(&dll, deepCopyTask(tasksArray[2]));
    insertAtBeginning(&dll, deepCopyTask(tasksArray[3]));

    printf("---------------------\n");

    parseListBeginningToEnd(dll);

    printf("---------------------\n");

    parseListEndToBeginning(dll);

    printf("---------------------\n");
    printf("---------------------\n");
    printf("---------------------\n");

    insertAtEnd(&dll, deepCopyTask(tasksArray[4]));
    insertAtEnd(&dll, deepCopyTask(tasksArray[1]));

    insertAtBeginning(&dll, deepCopyTask(tasksArray[0]));

    printf("---------------------\n");

    parseListBeginningToEnd(dll);

    printf("---------------------\n");

    parseListEndToBeginning(dll);

}