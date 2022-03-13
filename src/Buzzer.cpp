#include <Arduino.h>

#include "Buzzer.h"
#include "Output.h"
#include "Timer.h"

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
        tone(4, 2000, beepDuration);
        lastBeep = Time();
    }
}