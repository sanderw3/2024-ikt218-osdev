#include <stdint.h>
#include <stdio.h>
#include "Interrupts/IO/IO.h"
#include "Interrupts/PIT/PIT.h"
#include "Memory/memory.h"
#include "MusicPlayer/song.h"
#include "Drivers/keyboardIRQ.h"
#include "Extra/command.h"


// Existing global operator new overloads
void* operator new(size_t size) {
    return malloc(size);
}

void* operator new[](size_t size) {
    return malloc(size);
}

// Existing global operator delete overloads
void operator delete(void* ptr) noexcept {
    free(ptr);
}

void operator delete[](void* ptr) noexcept {
    free(ptr);
}

// Add sized-deallocation functions
void operator delete(void* ptr, size_t size) noexcept {
    (void)size; // Size parameter is unused, added to match required signature
    free(ptr);
}

void operator delete[](void* ptr, size_t size) noexcept {
    (void)size; // Size parameter is unused, added to match required signature
    free(ptr);
}



extern "C" int kernel_main(void);
int kernel_main(){
    setKeyboardNotUsed();


    // // Allocate some memory 
    // // using the kernel memory manager
    // // THIS IS PART OF THE ASSIGNMENT
    // void* some_memory = malloc(12345); 
    // void* memory2 = malloc(54321); 
    // void* memory3 = malloc(13331);
    // char* memory4 = new char[1000]();

    // int counter = 0;

    // while(true){
        // printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
        // sleep_busy(1000);
        // printf("[%d]: Slept using busy-waiting.\n", counter++);

        // printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
        // sleep_interrupt(1000);
        // printf("[%d]: Slept using interrupts.\n", counter++);
    // };

    // printf("Playing music...\n");






    // // create the song player
    // SongPlayer* player = create_song_player();
    
    // // make a list of all the songs to be played
    // Song* songs[] = {
    //     new Song({music_1, sizeof(music_1) / sizeof(Note)})
    // };

    // // get the number of songs
    // uint32_t n_songs = sizeof(songs) / sizeof(Song*);

    // // play the songs in an infinite loop
    // while(true){
    //     for(uint32_t i =0; i < n_songs; i++){
    //         printf("Playing Song...\n");
    //         player->play_song(songs[i]);
    //         printf("Finished playing the song.\n");
    //     }
    // };
    int counter = 10;

    while (!keyboardUsed() && counter-- > 0){
        moveToBeginningOfLine();
        printf("Playing music in %is", counter);
        sleep_interrupt(1000);
    }

    if (keyboardUsed()){
        moveToBeginningOfLine();
        printf("Type 'help' for commands!\n");
    } else{
        playSong();
        clearScreen();
        printf("Type 'help' for commands!\n");
    }

    return 0;
}