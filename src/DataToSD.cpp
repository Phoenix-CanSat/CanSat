#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include "DataToSD.h"

bool sd_init = false;

// Initializes SD card.
bool SDInit() {
    
    // If failed, send appropriate message.
    if (!SD.begin(BUILTIN_SDCARD)) {
        return false;
    }

    // Removes previously created files if they exist (only "data.csv" and "logger.txt").
    if (SD.exists("data.csv")) {
        SD.remove("data.csv");
    }
    if (SD.exists("logger.txt")) {
        SD.remove("logger.txt");
    }

    // Initialization successful.
    sd_init = true;

    // Text Initialization missed.
    SDWrite("\nInitializing Bob...\n", "logger.txt");
    // Data file header.
    SDWrite("Time,Temperature,Pressure,Latitude,Longitude,Altitude,Humidity,InternalTemperature", "data.csv");

    return sd_init;
}

// Store text to given file.
bool SDWrite(const char text[], const char filename[]) {

    if (sd_init) {
        // Open file with given file name.
        File file = SD.open(filename, FILE_WRITE);

        // Store text in the file and close the file.
        if (file) {
            file.println(text);
            file.close();
            return true;
        }
    }
    return false;
}