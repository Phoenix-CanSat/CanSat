#include <Arduino.h>

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
        Say("SD init.");
    } else {
        Say("SD not init.");
    }

    // Initializes RFM9X.
    if (RFInit()) {
        Say("RF init.");
    } else {
        Say("RF not init.");
    }

    // Initializes BME280.
    if (BMEInit()) {
        Say("BME280 init.");
    } else {
        Say("BME280 not init.");
    }

    // Initializes GPS.
    if (GPSInit()) {
        Say("GPS init.");
    } else {
        Say("GPS not init.");
    }

//--------------------------------------------------------------------------------------------------------------------------------------//
    
    Say("\nBob Initialized.\n");

}