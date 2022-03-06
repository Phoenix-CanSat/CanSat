#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include "DataToSD.h"
#include "Output.h"

bool sd_init = false;

// Initializes SD card.
bool SDInit() {
    
    // If failed, send appropriate message.
    if (!SD.begin(BUILTIN_SDCARD)) {
        return false;
    }

    // Removes previously created files if they exist (only "data" and "logger").
    if (SD.exists("data")) {
        SD.remove("data");
    }
    if (SD.exists("logger")) {
        SD.remove("logger");
    }

    // Initialization successful.
    sd_init = true;

    // Text Initialization missed.
    SDWrite("\nInitializing Bob...\n", "logger", false);
    // Data file header.
    SDWrite("Time,Temperature,Pressure,Latitude,Longitude,Altitude,Humidity,AccelerationX,AccelerationY,AccelerationZ,AngularVelocityX,AngularVelocityY,AngularVelocityZ,MagneticFieldStrengthX,MagneticFieldStrengthY,MagneticFieldStrengthZ", "data", true);

    return sd_init;
}

// Store text to given file.
bool SDWrite(const char text[], const char filename[], bool flush) {

    if (sd_init) {
        // Open file with given file name.
        File file = SD.open(filename, FILE_WRITE);

        // Store text in the file and close the file.
        if (file && file.available()) {
            file.println(text);
            if (flush == true) {
                file.flush();
            }
            file.close();
            return true;
        }
    }
    return false;
}