#ifndef DATATOSD_FUNCTIONS_H
#define DATATOSD_FUNCTIONS_H

#include <Arduino.h>
#include <SD.h>

/// Initializes SD card.
/// Removes previously created files with names "data" and "logger" if they exist.
/// @return true if initialization is successful.
/// @return false if initialization fails or an SD card is not connected.
bool SDInit();

/// Store text to given file.
/// @return true if text is stored.
/// @return false if file could not be opened or SD wasn't initialized.
bool SDWrite(const char text[], const char filename[], bool flush);

#endif