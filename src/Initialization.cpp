#include <Arduino.h>
#include <Wire.h>

#include "Initialization.h"
#include "Output.h"
#include "DataToSD.h"
#include "RF.h"
#include "ReadFromSensors.h"

void InitializeBob() {
    
    Say("\nInitializing Bob...\n");

//--------------------------------------------------------------------------------------------------------------------------------------//
    
    // Initializes micro SD card.
    if (SDInit()) {
        Say("SD initialized.");
    } else {
        Say("SD not initialized.");
    }

    // Initializes RFM9X.
    if (RFInit()) {
        Say("RF initialized.");
    } else {
        Say("RF not initialized.");
    }

    // Initializes BME280.
    if (BMEInit()) {
        Say("BME280 initialized.");
    } else {
        Say("BME280 not initialized.");
    }

    // Initializes GPS.
    if (GPSInit()) {
        Say("GPS initialized.");
    } else {
        Say("GPS not initialized.");
    }

    // Initializes BNO055.
    if (BNOInit()) {
        Say("BNO055 initialized.");
    } else {
        Say("BNO055 not initialized.");
    }

//--------------------------------------------------------------------------------------------------------------------------------------//
    
    Say("\nInitialization Completed.\n");

}