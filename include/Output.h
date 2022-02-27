#ifndef OUTPUT_FUNCTIONS_H
#define OUTPUT_FUNCTIONS_H

#include <Arduino.h>
#include <SD.h>

/// Prints the given text to the Serial (stores to buffer until it's full).
/// Stores it in "logger" file for debugging.
void Say(const char text[]);

/// Prints the given text to the Serial (flushes).
/// Stores it in "logger" file for debugging.
void SayNow(const char text[]);

#endif