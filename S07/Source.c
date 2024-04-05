#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define LINESIZE 128

typedef struct Task Task;
typedef struct Node Node;
typedef struct HashTable HT;

struct Task
{
    int taskId;
    char* description;
    float resourceUtil;
};

struct Node
{
    Task task;
    Node* next;
};

struct HashTable
{
    int size;
    Node** lists; // vector de liste
};

Task initTask(int taskId, char* description, float resourceUtil)
{
    Task task = { .taskId = taskId, .resourceUtil = resourceUtil };
    task.description = malloc(strlen(description) + 1);
    strcpy(task.description, description);

    return task;
}

Task readTaskFromFile(FILE* f)
{
    Task task;

    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    task.taskId = (int)strtol(buffer, NULL, 10);

    fgets(buffer, LINESIZE, f);
    buffer[strcspn(buffer, "\n")] = '\0'; // cauta poz unde gaseste un anumit caracter
    task.description = malloc(strlen(buffer) + 1);
    strcpy(task.description, buffer);

    fgets(buffer, LINESIZE, f);
    task.resourceUtil = strtof(buffer, NULL);

    return task;
}

void readTasksFromFile(const char* fileName, Task** tasksArray, int* tasksNumber)
{
    FILE* f = fopen(fileName, "r");

    int readTasks = 0;

    *tasksArray = malloc(readTasks * sizeof(Task));

    while (!feof(f))
    {
        Task task = readTaskFromFile(f);
        readTasks += 1;
        *tasksArray = realloc(*tasksArray, readTasks * sizeof(Task)); // realoc mem pt tasksArray
        (*tasksArray)[readTasks - 1] = task;
    }

    *tasksNumber = readTasks;
}

void printTaskToConsole(Task task)
{
    printf("Task %d - %s - %.2f%%\n", task.taskId, task.description, task.resourceUtil);
}

int hashFunction(int htSize, char* key) // key = description
{
    int sum = 0;
    for (int i = 0; i < strlen(key); i++)
    {
        sum += key[i];
    }

    return sum % htSize;
}

HT initHashTable(int size)
{
    HT ht;
    ht.size = size;
    ht.lists = malloc(size * sizeof(Node*)); // vector de liste
    for (int i = 0; i < ht.size; i++)
    {
        ht.lists[i] = NULL;
    }

    return ht;
}

void insertList(Node** head, Task task) // la final
{
    Node* newNode = malloc(sizeof(Node));
    newNode->task = task;
    newNode->next = NULL;

    if (*head)
    {
        Node* aux = *head;

        int different = strcmp(aux->task.description, task.description); // la fiecare nod parcurs verific daca e identic cu cel pe care vreau sa l introduc

        while (different && aux->next)
        {
            aux = aux->next;
            different = strcmp(aux->task.description, task.description);
        }

        if (!different)
        {
            // pe nodul in care s-a oprit while-ul
            free(aux->task.description);
            free(newNode);
            aux->task = task;
        }
        else
        {
            aux->next = newNode;
        }
    }
    else
    {
        *head = newNode;
    }
}

void insertHashTable(HT ht, Task task)
{
    int position = hashFunction(ht.size, task.description);

    if (ht.lists[position]) // facut doar pt a detecta coliziuni
    {
        //coliziune detectata
        insertList(&ht.lists[position], task);
    }
    else
    {
        // empty bucket - introducem elem in lista
        insertList(&ht.lists[position], task);
    }
}

void parseListAndPrint(Node* head)
{
    if (head)
    {
        while (head)
        {
            printf("\t");
            printTaskToConsole(head->task);
            head = head->next;
        }
    }
    else
    {
        printf("\tEmpty bucket\n");
    }
}

void printHashTable(HT ht) {
    for (int i = 0; i < ht.size; i++)
    {
        printf("Position %d\n", i);
        parseListAndPrint(ht.lists[i]);
    }
}

Task searchHashTable(HT ht, char* key)
{
    int position = hashFunction(ht.size, key);

    if (ht.lists[position]) // nu are sens if-ul si else-ul (e redundant)
    {
        // daca lista e populata parcurgem pana gasim elem (daca il gasim)
        Node* aux = ht.lists[position]; // primul elem din lista

        while (aux && strcmp(key, aux->task.description))
        {
            aux = aux->next;
        }

        if (aux)
        {
            printf("Found at position %d\n", position);
            return initTask(aux->task.taskId, aux->task.description, aux->task.resourceUtil);
        }
        else
        {
            Task error = { .taskId = 0, .description = "???", .resourceUtil = 0 };
            return error;
        }
    }
    else
    {
        Task error = { .taskId = 0, .description = "???", .resourceUtil = 0 };
        return error;
    }
}

int main() {

    int taskNumber;
    Task* tasksArray;

    readTasksFromFile("scheduler.txt", &tasksArray, &taskNumber);

    for (int i = 0; i < taskNumber; i++)
    {
        printTaskToConsole(tasksArray[i]);
    }

    printf("----------\n");

    HT ht = initHashTable(taskNumber);

    //insertHashTable(ht, tasksArray[0]); //-> shallow copy
    insertHashTable(ht, initTask(800, "Visual Studio", 40.21));
    insertHashTable(ht, initTask(tasksArray[0].taskId, tasksArray[0].description, tasksArray[0].resourceUtil));
    insertHashTable(ht, initTask(tasksArray[1].taskId, tasksArray[1].description, tasksArray[1].resourceUtil));
    insertHashTable(ht, initTask(tasksArray[2].taskId, tasksArray[2].description, tasksArray[2].resourceUtil));
    insertHashTable(ht, initTask(tasksArray[3].taskId, tasksArray[3].description, tasksArray[3].resourceUtil));
    insertHashTable(ht, initTask(tasksArray[4].taskId, tasksArray[4].description, tasksArray[4].resourceUtil));
    insertHashTable(ht, initTask(tasksArray[5].taskId, tasksArray[5].description, tasksArray[5].resourceUtil));
    insertHashTable(ht, initTask(tasksArray[6].taskId, tasksArray[6].description, tasksArray[6].resourceUtil));
    insertHashTable(ht, initTask(tasksArray[7].taskId, tasksArray[7].description, tasksArray[7].resourceUtil));

    printf("----------\n");

    printHashTable(ht);

    printf("----------\n");
    printf("----------\n");

    printTaskToConsole(searchHashTable(ht, "Antivirus"));

    printf("----------\n");
    printf("----------\n");

    insertHashTable(ht, initTask(1000, tasksArray[7].description, tasksArray[7].resourceUtil));

    printHashTable(ht);

    return 0;
}