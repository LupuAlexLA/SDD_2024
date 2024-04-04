#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LINESIZE 128
typedef struct Song Song;

struct Song
{
    int trackId;
    char* title;
    char* artist;
    float royalityPerPlay;
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

int main()
{
    int songsNumber;
    Song* songsArray;

    readSongsFromFile("playlist.txt", &songsArray, &songsNumber);

    for(int i = 0; i < songsNumber; i++)
    {
        printSongToConsole(songsArray[i]);
    }
}