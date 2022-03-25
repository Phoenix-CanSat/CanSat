#include <Arduino.h>

#include "Timer.h"

uint32_t initTime = 0;

// Sets the time in milliseconds (ms) after boot as the time since completion of initialization.
void CalculateInitTime() {
    initTime = millis();
}

// Returns the time in milliseconds (ms) since completion of initialization.
uint32_t Time() {
    return millis() - initTime;
}

// Waits untill the time since the moment given (second argument) is equal to or greater than the time of the first argument.
void Wait(uint32_t delayMS, uint32_t time) {
    if (Time() - time < delayMS) {
        delay(delayMS - (Time() - time));
    }
}