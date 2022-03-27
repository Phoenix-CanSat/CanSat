#include <Arduino.h>

#include "Output.h"
#include "DataToSD.h"

// Prints the given text to the Serial and stores it in "LOGGER" file for debugging.
void Say(const char text[]) {
    Serial.println(text);
    Serial.flush();
    SDWrite(text, "logger");
}