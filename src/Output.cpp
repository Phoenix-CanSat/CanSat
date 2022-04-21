#include <Arduino.h>

#include "Output.h"
#include "DataToSD.h"
#include "RF.h"

// Prints the given text to the Serial and stores it in "logger.txt" file for debugging.
void Say(const char text[]) {
    Serial.println(text);
    Serial.flush();
    SDWrite(text, "logger.txt");
}

// Prints the given text to the Serial, stores it in "logger.txt" file for debugging and sends it to the Ground Station.
void Shout(const char text[]) {
    delay(50);
    Serial.println(text);
    Serial.flush();
    SDWrite(text, "logger.txt");
    RFSendData(text, true);
}