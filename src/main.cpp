#include <Arduino.h>

#include "Initialization.h"
#include "Timer.h"
#include "Output.h"
#include "DataToSD.h"
#include "RF.h"
#include "ReadFromSensors.h"
#include "Buzzer.h"

// Minimum time between each loop.
#define loopDelay 500

//------------------------------------------------------------Initialization------------------------------------------------------------//

void setup() {

    // Initializes Serial.
    Serial.begin(115200);

    // Initializes Systems and Sensors.
    InitializeMel();

    // Calculates initialization time.
    CalculateInitTime();
}

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
    float internalTemperature = GetInternalTemperature();

    // Stores all data values to the data string and gets the length of the string.
    char data[200];
    snprintf(data, 200, "%lu,%.2f,%.2f,%.4f,%.4f,%.2f,%.2f,%.2f", time, temperature, pressure, latitude, longitude, altitude, humidity, internalTemperature);
    Serial.println(data);
    Serial.flush();

//--------------------------------------------------------Store Data To SD Card---------------------------------------------------------//

    // Saves data to "data.csv" file.
    if (SDWrite(data, "data.csv")) {
        Say("SD 1");
    } else {
        Say("SD 0");
    }

//-----------------------------------------------------Send Data To Ground Station------------------------------------------------------//

    // Sends data through RFM to receiver.
    if (RFSendData(data)) {
        Say("RF 1\n");
    } else {
        Say("RF 0\n");
    }

//----------------------------------------------------------------Buzzer----------------------------------------------------------------//

    // Sets what phase Mel is in (ascending/descending/landed).
    setPhase(altitude, time);

    // After landing, beep in set intervals.
    Beep();

//---------------------------------------------------------Delay Between Loops----------------------------------------------------------//
    
    // Makes sure there is a delay of at least 500ms between every loop.
    Wait(loopDelay, time);

    yield();
}