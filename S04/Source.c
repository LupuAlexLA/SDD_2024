#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LINESIZE 123

typedef struct Song Song;
typedef struct Node Node;

struct Song
{
    int trackId;
    char* title;
    char* artist;
    float royalityPerPlay;
};

struct Node
{
    Song song;
    Node* next;
};

void printSongToConsole(Song song)
{
    printf("-----\nSong ID: %d\nSong title: %sSong artist: %sRoyality per play: %.2f\n-----\n",
           song.trackId, song.title, song.artist, song.royalityPerPlay);
}

Song readSongFromFile(FILE* f)
{
    Song song;
    char buffer[LINESIZE];
    char* endptr = NULL;

    fgets(buffer, LINESIZE, f);
    song.trackId = (int)strtol(buffer, &endptr, 10);

    fgets(buffer, LINESIZE, f);
    song.title = (char*)malloc(strlen(buffer) + 1);
    strcpy(song.title, buffer);

    fgets(buffer, LINESIZE, f);
    song.artist = (char*)malloc(strlen(buffer) + 1);
    strcpy(song.artist, buffer);

    fgets(buffer, LINESIZE, f);
    song.royalityPerPlay = (float)strtof(buffer, &endptr);

    return song;
}

void readSongsFromFile(const char* fileName, Song** songsArray, int* songsNumber)
{
    FILE* f = fopen(fileName, "r");
    char buffer[LINESIZE];
    char* endptr = NULL;

    fgets(buffer, LINESIZE, f);
    *songsNumber = (int)strtol(buffer, &endptr, 10);

    *songsArray = malloc((*songsNumber) * sizeof(Song));

    for(int i = 0; i < (*songsNumber); i++)
    {
        (*songsArray)[i] = readSongFromFile(f);
    }
}

void insertAtBeginning(Node** head, Song song)
{
    Node* newNode = malloc(sizeof(Node));

    newNode->song = song;
    newNode->next = *head;

    *head = newNode;
}

void insertAtEnd(Node** head, Song song)
{
    if(*head)
    {
        Node* aux = *head;
        while(aux->next)
        {
            aux = aux->next;
        }

        Node* newNode = malloc(sizeof(Node));
        newNode->song = song;
        newNode->next = NULL;

        aux->next = newNode;
    }
    else
    {
        insertAtBeginning(head, song);
    }
}

void parseListAndPrint(Node* head)
{
    if(head)
    {
        while(head)
        {
            printSongToConsole(head->song);
            head = head->next;
        }
    }
    else
    {
        printf("List is empty\n");
    }
}

int main() {

    int songsNumber;
    struct Song* songsArray;

    readSongsFromFile("playlist.txt", &songsArray, &songsNumber);

    /*for (int i = 0; i < songsNumber; i++) {
        printSongToConsole(songsArray[i]);
    }*/

    struct Node* head = NULL;

    parseListAndPrint(head);


    printf("\n\nList after one insert:\n");
    insertAtBeginning(&head, songsArray[0]);
    parseListAndPrint(head);
    printf("------------------------\n\n");

    printf("\n\nList after the second insert:\n");
    insertAtBeginning(&head, songsArray[1]);
    parseListAndPrint(head);
    printf("------------------------\n\n");

    printf("\n\nList after the third insert:\n");
    insertAtBeginning(&head, songsArray[2]);
    parseListAndPrint(head);
    printf("------------------------\n\n");

    struct Node* head2 = NULL;

    for (int i = 0; i < songsNumber; i++) {
        insertAtBeginning(&head2, songsArray[i]);
    }

    printf("\n\nList after inserting all songs:\n");
    parseListAndPrint(head2);


    printf("\n\nINSERTING AT THE END:\n");

    struct Node* head3 = NULL;

    parseListAndPrint(head3);


    printf("\n\nList after one insert:\n");
    insertAtEnd(&head3, songsArray[0]);
    parseListAndPrint(head3);
    printf("------------------------\n\n");

    printf("\n\nList after the second insert:\n");
    insertAtEnd(&head3, songsArray[1]);
    parseListAndPrint(head3);
    printf("------------------------\n\n");

    printf("\n\nList after the third insert:\n");
    insertAtEnd(&head3, songsArray[2]);
    parseListAndPrint(head3);
    printf("------------------------\n\n");

    printf("\n\nList after inserting all songs:\n");
    insertAtEnd(&head3, songsArray[3]);
    parseListAndPrint(head3);
    printf("------------------------\n\n");

    return 0;
}