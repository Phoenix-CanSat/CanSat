#include <Arduino.h>
#include <Wire.h>

#include "Initialization.h"
#include "Time.h"
#include "Output.h"
#include "DataToSD.h"
#include "RF.h"
#include "ReadFromSensors.h"

// Sets minimum time between each loop.
#define loopDelay 250

#define descentSamples 12
#define stillSamples 4

// Set buzzer beeping.
#define beepDuration 1000
#define beep() tone(4, 2000, beepDuration)

float lastAltitude = 0;
uint32_t lastTime = 0;
uint8_t descend = 0;
bool isDescending = false;
uint8_t still = 0;
bool buzzer = false;
uint32_t lastBeep = 0;

//----------------------------------------------------------Initialize Sensors----------------------------------------------------------//

void setup() {
    while (!Serial);
    // Initializes Serial and CanSat.
    Serial.begin(115200);
    InitializeBob();
    // Calculates initialization time.
    CalculateInitTime();
}

//----------------------------------------------------------Enter Endless Loop----------------------------------------------------------//

void loop() {

//--------------------------------------------------------Get Data From Sensors---------------------------------------------------------//
    
    // Time data is read.
    uint32_t time = Time();

    // Stores sensor values to appropriate variables.
    float temperature = GetTemperature();
    float pressure = GetPressure();
    float latitude = GetLatitude();
    float longitude = GetLongitude();
    float altitude = GetAltitude();
    float humidity = GetHumidity();
    float acceleration[3] = {GetAcceleration(X), GetAcceleration(Y), GetAcceleration(Z)};
    float gyroscope[3] = {GetGyroscope(X), GetGyroscope(Y), GetGyroscope(Z)};
    float magnetic[3] = {GetMagnetic(X), GetMagnetic(Y), GetMagnetic(Z)};
    /// TODO:
    ///  chiptemperature

    // Stores all data values to the data string and gets the length of the string.
    char data[225];
    uint8_t datalen = snprintf(data, 225, "%lu,%.2f,%.2f,%.4f,%.4f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f", time, temperature, pressure, latitude, longitude, altitude, humidity, acceleration[X], acceleration[Y], acceleration[Z], gyroscope[X], gyroscope[Y], gyroscope[Z], magnetic[X], magnetic[Y], magnetic[Z]);
    SayNow(data);

//--------------------------------------------------------Store Data To SD Card---------------------------------------------------------//

    // Saves data to "data" file.
    if (SDWrite(data, "data", true)) {
        Say("\nData successfully stored in SD.");
    } else {
        Say("\nData failed to be stored in SD.");
    }

//-----------------------------------------------------Send Data To Ground Station------------------------------------------------------//

    // Sends data through RFM to receiver.
    if (RFSendData(data, datalen)) {
        Say("RF successfully sent data.\n");
    } else {
        Say("RF failed to send data.\n");
    }

//-------------------------------------------------------Start Buzzer (If Landed)-------------------------------------------------------//

    // Checks if buzzer is already on.
    if (buzzer == false) {
        // Calculates descending speed.
        float velocity = (altitude - lastAltitude) / (float)(time - lastTime);

        // Checks if Bob is descending.
        if (isDescending == false) {
            if (velocity <= -1 && descend < descentSamples) {
                descend += 1;
            } else if (velocity > -1 && descend < descentSamples) {
                descend = 0;
            } else {
                isDescending = true;
            }
        } else {
            // When landed, sends message and starts the buzzer.
            if (still < stillSamples && abs(velocity) < 1) {
                still += 1;
            } else if (still < stillSamples && abs(velocity) >= 1) {
                still = 0;
            } else if (still == stillSamples && abs(velocity) < 1) {
                Say("Bob has landed.\n");
                buzzer = true;
            }
        }

        lastAltitude = altitude;
        lastTime = time;
    }

    // After landing, beep in set intervals.
    if (buzzer == true && Time() - lastBeep >= 2 * beepDuration) {
        beep();
        lastBeep = Time();
    }

//---------------------------------------------------------Delay Between Loops----------------------------------------------------------//
    
    // Makes sure there is a delay of at least 250ms between every loop.
    if (Time() - time < loopDelay) {
        delay(loopDelay - (Time() - time));
    }

    yield();
}

/// TODO:
///     Test GPS with battery.
///     Program Flight Controller (Betaflight).
///     Create SD support for Ground Station.
///     Add Identifier with Packet Counter before every packet.
///     Add support for Packet Identifiers in Ground Station.
///     Visualise BNO055 data for orientation in 3D.
///     Calibrate Chip Temperature.