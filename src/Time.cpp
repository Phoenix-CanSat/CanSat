#include <Arduino.h>

#include "Time.h"

int initTime = 0;

// Sets the time in milliseconds (ms) after boot as the time since completion of initialization.
void CalculateInitTime() {
    initTime = millis();
}

// Returns the time in milliseconds (ms) since completion of initialization.
int Time() {
    return millis() - initTime;
}