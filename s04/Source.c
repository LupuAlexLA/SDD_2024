#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

#define LINESIZE 128
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
	char buffer[LINESIZE];

	char* endptr = NULL;
	Song song;

	fgets(buffer, LINESIZE, f);
	song.trackId = (int)strtol(buffer, &endptr, 10);

	fgets(buffer, LINESIZE, f);
	song.title = malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy_s(song.title, strlen(buffer) + 1, buffer);

	fgets(buffer, LINESIZE, f);
	song.artist = malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy_s(song.artist, strlen(buffer) + 1, buffer);

	fgets(buffer, LINESIZE, f);
	song.royalityPerPlay = strtof(buffer, &endptr, 10);

	return song;
}

void readSongsFromFile(const char* fileName, Song** songsArray, int* songsNumber)
{
	FILE* f;
	fopen_s(&f, fileName, "r");

	char buffer[LINESIZE];
	char* endptr = NULL;

	fgets(buffer, LINESIZE, f);
	*songsNumber = (int)strtol(buffer, &endptr, 10);

	*songsArray = malloc((*songsNumber) * sizeof(Song));

	for (int i = 0; i < *songsNumber; i++)
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
	if (*head)
	{
		Node* aux = *head;
		while (aux->next)
		{
			aux = aux->next; // parsez lista
		}

		Node* newNode = malloc(sizeof(Node));
		newNode->song = song;
		newNode->next = NULL; // poate fi si aux->next (e null)

		aux->next = newNode;
	}
	else // insertAtBeginning
	{
		Node* newNode = malloc(sizeof(Node));

		newNode->song = song;
		newNode->next = *head; // poate fi si NULL (head e null)

		*head = newNode;
	}
}

void parseListAndPrint(Node* head)
{
	if (head) // <=> if(head != NULL)
	{
		while (head)
		{
			printSongToConsole(head->song);
			head = head->next;
		}
	}
	else 
	{
		printf("List is empty!\n");
	}
}

int main()
{
	int songsNumber;
	Song* songsArray;

	readSongsFromFile("playlist.txt", &songsArray, &songsNumber);

	for (int i = 0; i < songsNumber; i++)
		printSongToConsole(songsArray[i]);

	Node* head = NULL;

	insertAtBeginning(&head, songsArray[0]);
	parseListAndPrint(head);

	printf("\n\nfirst\n\n");

	insertAtBeginning(&head, songsArray[1]);
	parseListAndPrint(head);

	printf("\n\second\n\n");

	insertAtBeginning(&head, songsArray[2]);
	parseListAndPrint(head);

	printf("\n\nthird\n\n");

	Node* head2 = NULL;

	for (int i = 0; i < songsNumber; i++)
	{
		insertAtBeginning(&head2, songsArray[i]);
	}
	parseListAndPrint(head2);
	printf("\n\nfourth\n\n");

	Node* head3 = NULL;

	printf("\n\nThird LIST\n\n");

	insertAtEnd(&head3, songsArray[0]);
	parseListAndPrint(head3);

	printf("\n\nfirst\n\n");

	insertAtEnd(&head3, songsArray[1]);
	parseListAndPrint(head3);

	printf("\n\second\n\n");

	insertAtEnd(&head3, songsArray[2]);
	parseListAndPrint(head3);

	printf("\n\nthird\n\n");

	return 0;
}