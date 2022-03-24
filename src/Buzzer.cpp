#include <Arduino.h>

#include "Buzzer.h"
#include "Output.h"
#include "Timer.h"

#define BUZZER_CS 4

#define NOTE_A4 440
#define NOTE_B4 494
#define NOTE_D5 587
#define NOTE_E5 659
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_A5 880
#define REST 0

#define Tempo 114

// Duration of a whole note in ms
#define wholenote 60000*4/Tempo

#define descentSamples 12
#define stillSamples 4

// Set buzzer beeping.
#define beepDuration 1000

bool buzzer = false;

float lastAltitude = 0;
uint32_t lastTime = 0;
uint8_t descend = 0;
bool isDescending = false;
uint8_t still = 0;

uint32_t lastBeep = 0;

// Notes of the melody followed by their duration
int melody[] = {

    NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 4,
    NOTE_E5, -4, NOTE_FS5, -4, NOTE_A5, 16, NOTE_G5, 16, NOTE_FS5, 8,
    NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 2,
    NOTE_A4, 16, NOTE_A4, 16, NOTE_B4, 16, NOTE_D5, 8, NOTE_D5, 16, REST, 4

};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

int divider = 0, noteDuration = 0;

void initNotif() {
    
    for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
        // calculates the duration of each note
        divider = melody[thisNote + 1];
        if (divider > 0) {
            // regular note, just proceed
            noteDuration = wholenote / divider;
        } else if (divider < 0) {
            // dotted notes are represented with negative durations!!
            noteDuration = wholenote / abs(divider);
            noteDuration *= 1.5;  // increases the duration in half for dotted notes
        }

        // we only play the note for 90% of the duration, leaving 10% as a pause
        tone(BUZZER_CS, melody[thisNote], noteDuration * 0.9);

        // Wait for the specief duration before playing the next note.
        delay(noteDuration);

        // stop the waveform generation before the next note.
        noTone(BUZZER_CS);
    }
}

void setPhase(float alt, uint32_t time) {
    // Checks if buzzer is already on.
    if (buzzer == false) {
        // Calculates descending speed.
        float velocity = (alt - lastAltitude) / (float)(time - lastTime);

        // Checks if Bob is descending.
        if (isDescending == false) {
            if (velocity <= -1 && descend < descentSamples) {
                descend += 1;
            } else if (velocity > -1 && descend < descentSamples) {
                descend = 0;
            } else {
                Say("Bob descending.\n");
                isDescending = true;
            }
        } else {
            // When landed, sends message and starts the buzzer.
            if (still < stillSamples && abs(velocity) < 1) {
                still += 1;
            } else if (still < stillSamples && abs(velocity) >= 1) {
                still = 0;
            } else if (still == stillSamples && abs(velocity) < 1) {
                Say("Bob landed.\n");
                buzzer = true;
            }
        }

        lastAltitude = alt;
        lastTime = time;
    }
}

void Beep() {
    if (buzzer == true && Time() - lastBeep >= 2 * beepDuration) {
        tone(BUZZER_CS, 3000, beepDuration);
        lastBeep = Time();
    }
}