#ifndef OUTPUT_FUNCTIONS_H
#define OUTPUT_FUNCTIONS_H

#include <Arduino.h>

/// Prints the given text to the Serial.
/// Stores it in "logger.txt" file for debugging.
void Say(const char text[]);

#endif