#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "MusicPlayer/song.h"
#include "Memory/memory.h"
#include "Interrupts/PIT/PIT.h"
#include "Extra/math.h"
#include "Interrupts/PIC/PIC.h"
#include "Drivers/keyboardIRQ.h"



// from https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function:

constexpr uint32_t fnv1a_hash(const char *str, uint32_t hash = 2166136261U) {
    return *str ? fnv1a_hash(str + 1, (hash ^ static_cast<uint32_t>(*str)) * 16777619U) : hash;
}



void printHelp(){
    printf("\n\n");
    printf("--- Available commands: ---\n");
    printf("\n");
    printf("help: prints this help message\n");
    printf("clear: clears the screen\n");
    printf("play: plays a song\n");
    printf("testinterrupt: calls 3 dummy interrupts\n");
    printf("testallocation: allocates some memory for testing (freed after)\n");
    printf("home: prints the splash screen\n");
    printf("\n");
    printf("---------------------------\n");
    printf("\n");
}




void playSong(){
    printf("\n");

    SongPlayer* player = create_song_player();
    
    // make a list of all the songs to be played
    Song* songs[] = {
        new Song({music_4, sizeof(music_4) / sizeof(Note)})
    };

    // get the number of songs
    uint32_t n_songs = sizeof(songs) / sizeof(Song*);

    for(uint32_t i =0; i < n_songs; i++){
        printf("\nPlaying Song...\n");
        player->play_song(songs[i]);
        printf("Finished playing the song.\n");

        // Deallocate the song after it is played
        delete songs[i];
    }

    // Deallocate the player
    delete player;
}









// const int screen_width = 80;
// const int screen_height = 22;
// const float theta_spacing = 0.07;
// const float phi_spacing = 0.02;
// const int sleep_duration = 30000;  // in microseconds

// void render_frame(char* buffer, float A, float B) {
//     memset(buffer, ' ', screen_width * screen_height);
//     buffer[screen_width * screen_height] = '\0';

//     float z_buffer[screen_width * screen_height];
//     memset(z_buffer, 0, sizeof(z_buffer));

//     for (float theta = 0; theta < 2 * 3.1415926535; theta += theta_spacing) {
//         for (float phi = 0; phi < 2 * 3.1415926535; phi += phi_spacing) {
//             float cos_theta = my_cos(theta), sin_theta = my_sin(theta);
//             float cos_phi = my_cos(phi), sin_phi = my_sin(phi);
//             float cos_A = my_cos(A), sin_A = my_sin(A);
//             float cos_B = my_cos(B), sin_B = my_sin(B);

//             float circle_x = cos_theta;
//             float circle_y = sin_theta;
//             float x = cos_B * circle_x + sin_A * sin_B * circle_y;
//             float y = cos_A * circle_y - sin_A * circle_x * cos_B;
//             float ooz = 1 / (cos_theta + 2);
//             float z = cos_phi * ooz;

//             int xp = (int)(screen_width / 2 + 30 * ooz * (cos_phi * x - sin_phi * y));
//             int yp = (int)(screen_height / 2 + 15 * ooz * (cos_phi * x + sin_phi * y));
//             int idx = xp + screen_width * yp;

//             if (0 <= idx && idx < screen_width * screen_height) {
//                 if (ooz > z_buffer[idx]) {
//                     z_buffer[idx] = ooz;
//                     buffer[idx] = ".,-~:;=!*#$@"[(int)(12 * (ooz - 0.6))];
//                 }
//             }
//         }
//     }

//     printf("\x1b[H%s", buffer);
// }












extern "C" void performCommand(const char* command);

void performCommand(const char* command) {
    if (!keyboardNotFinished()) {
        playSong();
        return;
    }

    switch (fnv1a_hash(command)){

    case fnv1a_hash("help"):
        printHelp();
        break;

    case fnv1a_hash("clear"):
        clearScreen();
        break;

    case fnv1a_hash("play"):
        playSong();
        break;

    case fnv1a_hash("testinterrupt"):
        __asm__ volatile ("int $0x01");
        __asm__ volatile ("int $0x02");
        __asm__ volatile ("int $0x00");
        break;
    
    case fnv1a_hash("testallocation"):
    {
        // Allocate some memory 
        // using the kernel memory manager
        // THIS IS PART OF THE ASSIGNMENT
        void* some_memory = malloc(12345); 
        void* memory2 = malloc(54321); 
        void* memory3 = malloc(13331);
        char* memory4 = new char[1000]();
        free(some_memory);
        free(memory2);
        free(memory3);
        delete[] memory4;
        break;
    }

    case fnv1a_hash("home"):\
        clearScreen();
        Set_text_color(RED);
        mask_pic(1);
        printSplashScreen();
        unmask_pic(1);
        Set_text_color(DEFAULT);
        break;

    // case fnv1a_hash("donut"):
    // {
    //     char buffer[screen_width * screen_height + 1];
    //     float A = 0, B = 0;
    //     while (1) {
    //         render_frame(buffer, A, B);
    //         A += theta_spacing;
    //         B += phi_spacing;
    //         sleep_interrupt(sleep_duration);
    //     }
    //     break;
    // }
    default:
        printf("\nUnknown command: ");
        printf("\"%s\"", command);
        printf(", type 'help' if you need assistance.");
        break;
    }
}



