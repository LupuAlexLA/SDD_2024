#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct Song Song;
#define LINESIZE 128

struct Song 
{
    int trackId;
    char* title;
    char* artist;
    float royalityPerPlay;
};

Song initSong(int trackId, char* title, char* artist, float royalityPerPlay)
{
    Song song;

    song.trackId = trackId;
    song.title = (char*)malloc(strlen(title) + 1);
    strcpy(song.title, title);
    song.artist = (char*)malloc(strlen(artist) + 1);
    strcpy(song.artist, artist);
    song.royalityPerPlay = royalityPerPlay;

    return song;
}

void printSong(Song song) 
{
    printf("---------\nSong ID: %d\nSong title: %s\nSong artist: %s\nRoyality per play: %.3f\n---------\n",
    song.trackId, song.title, song.artist, song.royalityPerPlay);
}

Song readSongFromFile(FILE* f)
{
    Song song;

    char buffer[LINESIZE];
    char* endptr = NULL;  // for strtol and strof, store the rest of the string
    char* songData = NULL; // saves the token at each call of strtok

    fgets(buffer, LINESIZE, f); // reads from file (f) a char* (buffer), with size = LINESIZE

    songData = strtok(buffer, ","); // reads the line with separator
    song.trackId = (int)strtol(songData, &endptr, 10); // convert str to long

    songData = strtok(NULL, ",");
    song.title = (char*)malloc(strlen(songData) + 1);
    strcpy(song.title, songData);

    songData = strtok(NULL, ",");
    song.artist = (char*)malloc(strlen(songData) + 1);
    strcpy(song.artist, songData);

    songData = strtok(NULL, ",");
    song.royalityPerPlay = strtof(songData, &endptr);

    return song;
}

Song* readSongsFromFile(char* fileName, int* songsNo)
{
    FILE* f;
    f = fopen(fileName, "r");

    char buffer[LINESIZE];
    fgets(buffer, LINESIZE, f);

    char* endptr = NULL; 
    *songsNo = (int)strtol(buffer, &endptr, 10); // str to long

    Song* songs = malloc(*songsNo * sizeof(Song));

    for(int i = 0; i < (*songsNo); i++) 
    {
        songs[i] = readSongFromFile(f);
    }

    fclose(f);

    return songs;
}

int main()
{
    Song song1 = initSong(0, "Sandman", "Metallica", 0.586);
    printSong(song1);

    int songsNo;
    Song* songs = readSongsFromFile("songs.txt", &songsNo);

    for(int i = 0; i < songsNo; i++) 
    {
        printSong(songs[i]);
    }

    FILE* f;
    f = fopen("songs.txt", "r");
    int songsNumber;
    fscanf(f, "%d", &songsNumber);
    printf("Songs number: %d", songsNumber);
}

// tema: citirea cu abordarea, un nr/cuvant pe cate un rand

// comenzi git clonare

// git clone
// (creare branch nou): git checkout -d nume
// (upload sursa sau tot proiectu in homework) git add; git commit -m mesaj; git push