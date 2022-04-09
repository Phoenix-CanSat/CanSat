#include <Arduino.h>

#include "Initialization.h"
#include "Output.h"
#include "DataToSD.h"
#include "RF.h"
#include "ReadFromSensors.h"
#include "Buzzer.h"

void InitializeBob() {
    
    Say("\nInitializing Bob...\n");

//--------------------------------------------------------------------------------------------------------------------------------------//
    
    // Initializes micro SD card.
    if (SDInit()) {
        Say("SD init.");
        Init(true);
    } else {
        Say("SD not init.");
        Init(false);
    }

    // Initializes RFM9X.
    if (RFInit()) {
        Say("RF init.");
        Init(true);
    } else {
        Say("RF not init.");
        Init(false);
    }

    // Initializes BME280.
    if (BMEInit()) {
        Say("BME280 init.");
        Init(true);
    } else {
        Say("BME280 not init.");
        Init(false);
    }

    // Initializes GPS.
    if (GPSInit()) {
        Say("GPS init.");
        Init(true);
    } else {
        Say("GPS not init.");
        Init(false);
    }

//--------------------------------------------------------------------------------------------------------------------------------------//

    Say("\nBob Initialized.\n");

    // Notifies when initialization is complete.
    initNotif();
    
}