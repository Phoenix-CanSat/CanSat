#include <Arduino.h>
#include <Wire.h>

#include "Initialization.h"
#include "Time.h"
#include "Output.h"
#include "DataToSD.h"
#include "RF.h"
#include "ReadFromSensors.h"

// Time between loops
#define DELAY 300
// Estimated time needed for landing.
#define landingTime 20000

// Set buzzer beeping.
#define beepDuration 1000
#define beep() tone(4, 2000, beepDuration)

bool landed = false;
uint32_t lastBeep = 0;

void setup() {
    while (!Serial);
    // Initializes Serial and CanSat.
    Serial.begin(115200);
    InitializeBob();
    CalculateInitTime();
}

void loop() {
    
    // Time data was read.
    int time = Time();

    // Stores sensor values to appropriate variables.
    float temperature = GetTemperature();
    float pressure = GetPressure();
    float latitude = GetLatitude();
    float longitude = GetLongitude();
    float altitude = GetAltitude();
    float humidity = GetHumidity();
    float magnetic[3] = {GetMagnetic(X), GetMagnetic(Y), GetMagnetic(Z)};
    float gravity[3] = {GetGravity(X), GetGravity(Y), GetGravity(Z)};
    // TODO:
    //  chiptemperature

    // Stores all data values to the data string and gets the length of the string.
    char data[225];
    uint8_t datalen = snprintf(data, 225, "%d,%.2f,%.2f,%.4f,%.4f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f", time, temperature, pressure, latitude, longitude, altitude, humidity, magnetic[X], magnetic[Y], magnetic[Z], gravity[X], gravity[Y], gravity[Z]);
    SayNow(data);

    // Saves data to "data" file.
    if (SDWrite(data, "data", true)) {
        Say("\nData successfully stored in SD.");
    } else {
        Say("\nData failed to be stored in SD.");
    }

    // Sends data through RFM to receiver.
    if (RFSendData(data, datalen)) {
        Say("RF successfully sent data.\n");
    } else {
        Say("RF failed to send data.\n");
    }

//--------------------------------------------------------------------------------------------------------------------------------------//
    
    // When landed, sends message and sets "landed" variable to true.
    if (landed==false && time>=landingTime) {
        Say("Bob has landed.\n");
        landed = true;
    }

    // After landing:
    if (landed==true) {
        // Beep in set intervals.
        if (time-lastBeep>=2*beepDuration) {
            beep();
            lastBeep = time;
        }
    }
    
    // Makes sure there is a delay between every packet.
    uint32_t interval = millis() - time;
    if (interval <= DELAY) {
        delay(DELAY - interval);
    }

    yield();
}