#include "MusicPlayer/song.h"
#include "Interrupts/PIT/PIT.h"
#include "Interrupts/IO/IO.h"
#include "libc/stdio.h"

void enable_speaker(){
    // 1. Read the current state from the PC speaker control port.
    uint8_t speaker_state = inputb(PC_SPEAKER_PORT);
    /*
    Bit 0: Speaker gate
            0: Speaker disabled
            1: Speaker enabled
    Bit 1: Speaker data
            0: Data is not passed to the speaker
            1: Data is passed to the speaker
    */
    // Check if bits 0 and 1 are not set (0 means that the speaker is disabled)
    if ((speaker_state & 3) != 3) {
        // If bits 0 and 1 are not set, enable the speaker by setting bits 0 and 1 to 1
        outputb(PC_SPEAKER_PORT, speaker_state | 3);
    }
}

void disable_speaker() {
    // Turn off the PC speaker
    // 1. Read the current state from the PC speaker control port.
    uint8_t speaker_state = inputb(PC_SPEAKER_PORT);

    // 2. Clear both Bit 0 and Bit 1 to disable the speaker.
    outputb(PC_SPEAKER_PORT, speaker_state & 0b11111100);
}

void play_sound(uint32_t frequency) {
    if (frequency == 0) {
        return;
    }
    
    // Calculate the divisor for the desired frequency
    uint16_t divisor = (uint16_t)(PIT_BASE_FREQUENCY / frequency);

    // Send the control word to the PIT control port
    outputb(PIT_CMD_PORT, 0b10110110); // 0xB6, channel 2, access mode lobyte/hibyte, mode 3 (square wave generator)

    // Send the frequency divisor to the PIT channel 2 data port
    outputb(PIT_CHANNEL2_PORT, (uint8_t)(divisor & 0xFF)); // Low byte
    outputb(PIT_CHANNEL2_PORT, (uint8_t)((divisor >> 8) & 0xFF)); // High byte

    // Enable and play the sound
    enable_speaker();

}

void stop_sound() {
    // 1. Read the current state from the PC speaker control port.
    uint8_t speaker_state = inputb(PC_SPEAKER_PORT);


    // 2. Clear the bit that enables speaker data to stop the sound.
    //    - Use bitwise AND with the complement of the bit responsible for enabling the speaker data.
    outputb(PC_SPEAKER_PORT, speaker_state & 0b11111101);

}


void play_whole_song(Song *song) {
    // 1. Enable the speaker before starting the song.
    enable_speaker();

    // 2. Loop through each note in the song's notes array:
    for (uint32_t i = 0; i < song->length; i++) {
        Note* note = &song->notes[i];
        //    a. For each note, display its details such as frequency and duration.
        printf("Note: %d, Freq=%d, Sleep=%d\n", i, note->frequency, note->duration);

        //    b. Call play_sound with the note's frequency.
        play_sound(note->frequency);

        //    c. Delay execution for the duration of the note (this can be implemented with a sleep function).
        sleep_interrupt(note->duration);

        //    d. Call stop_sound to end the note.
        stop_sound();

        // if the frequency is 0, we still need to sleep for the duration of the note
        sleep_interrupt(note->duration * (note->frequency==0)); 
    }

    // 3. Disable the speaker after all notes have been played.
    disable_speaker();
}


void play_song(Song *song) {
    // 1. Call play_whole_song with the given song.
    play_whole_song(song);
}


SongPlayer* create_song_player() {
    SongPlayer* player = new SongPlayer();
    player->play_song = play_song;
    return player;
}