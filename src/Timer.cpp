#include <Arduino.h>

#include "Timer.h"

// Sets minimum time between each loop.
#define loopDelay 250

uint32_t initTime = 0;

// Sets the time in milliseconds (ms) after boot as the time since completion of initialization.
void CalculateInitTime() {
    initTime = millis();
}

// Returns the time in milliseconds (ms) since completion of initialization.
uint32_t Time() {
    return millis() - initTime;
}

// Makes sure there is a delay of at least 250ms between every loop.
void Wait(uint32_t time) {
    if (Time() - time < loopDelay) {
        delay(loopDelay - (Time() - time));
    }
}