#include <Arduino.h>

#include "Buzzer.h"
#include "Output.h"
#include "Timer.h"

#define BUZZER_CS 5

#define NOTE_A4 440
#define NOTE_D5 587
#define NOTE_E5 659
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_A5 880
#define REST 0

#define Tempo 114

// Duration of a whole note in ms
#define wholenote 60000*4/Tempo

#define descentSamples 10
#define stillSamples 10

#define fallingVelocityLimit 2

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
    NOTE_D5, -4, NOTE_E5, -4, NOTE_A4, 2, REST, 4

};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

int divider = 0, noteDuration = 0;

// Plays appropriate sound depending on wether a system is initialized or not.
void Init(bool success) {
    if (success) {
        tone(BUZZER_CS, NOTE_A5, beepDuration/2);
    } else {
        tone(BUZZER_CS, NOTE_A4, beepDuration/2);
    }
    delay(beepDuration);
}

// Rickrolls the person who turns on Bob.
void initNotif() {
    
    for (int thisNote = 0; thisNote < notes * 2; thisNote += 2) {
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

// Sets what phase Bob is in (ascending/descending/landed).
void setPhase(float alt, uint32_t time) {
    // Checks if buzzer is already on.
    if (buzzer == false) {
        // Calculates descending speed.
        float fallingVelocity = (alt - lastAltitude) / (float)(time - lastTime);

        // Checks if Bob is descending.
        if (isDescending == false) {
            if (fallingVelocity >= fallingVelocityLimit && descend < descentSamples) {
                descend += 1;
            } else if (fallingVelocity < fallingVelocityLimit && descend < descentSamples) {
                descend = 0;
            } else {
                Say("Bob descending.\n");
                isDescending = true;
            }
        } else {
            // When landed, sends message and starts the buzzer.
            if (still < stillSamples && abs(fallingVelocity) < fallingVelocityLimit) {
                still += 1;
            } else if (still < stillSamples && abs(fallingVelocity) >= fallingVelocityLimit) {
                still = 0;
            } else if (still == stillSamples && abs(fallingVelocity) < fallingVelocityLimit) {
                Say("Bob landed.\n");
                buzzer = true;
            }
        }

        lastAltitude = alt;
        lastTime = time;
    }
}

// After landing, beep in set intervals.
void Beep() {
    if (buzzer == true && Time() - lastBeep >= 2 * beepDuration) {
        tone(BUZZER_CS, 4000, beepDuration);
        lastBeep = Time();
    }
}