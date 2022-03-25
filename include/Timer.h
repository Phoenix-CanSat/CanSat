#ifndef TIMER_FUNCTIONS_H
#define TIMER_FUNCTIONS_H

#include <Arduino.h>

/// Sets the time in milliseconds (ms) after boot as the time since completion of initialization.
void CalculateInitTime();

/// @return the time in milliseconds (ms) since completion of initialization.
uint32_t Time();

// Waits untill the time since the moment given (second argument) is equal to or greater than the time of the first argument.
void Wait(uint32_t delayMS, uint32_t time);

#endif