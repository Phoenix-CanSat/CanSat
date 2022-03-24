#include <Arduino.h>

#include "Initialization.h"
#include "Timer.h"
#include "Output.h"
#include "DataToSD.h"
#include "RF.h"
#include "ReadFromSensors.h"
#include "Buzzer.h"

//----------------------------------------------------------Initialize Sensors----------------------------------------------------------//

void setup() {

    // Initializes Serial.
    while (!Serial);
    Serial.begin(115200);

    // Initializes CanSat.
    InitializeBob();

    // Notifies when initialization is complete.
    initNotif();

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
    /// TODO:
    ///  chiptemperature

    // Stores all data values to the data string and gets the length of the string.
    char data[225];
    uint8_t datalen = snprintf(data, 225, "%lu,%.2f,%.2f,%.4f,%.4f,%.2f,%.2f", time, temperature, pressure, latitude, longitude, altitude, humidity);
    SayNow(data);

//--------------------------------------------------------Store Data To SD Card---------------------------------------------------------//

    // Saves data to "data" file.
    if (SDWrite(data, "data", true)) {
        Say("\nSD saved.");
    } else {
        Say("\nSD failed.");
    }

//-----------------------------------------------------Send Data To Ground Station------------------------------------------------------//

    // Sends data through RFM to receiver.
    if (RFSendData(data, datalen)) {
        Say("RF sent.\n");
    } else {
        Say("RF failed.\n");
    }

//----------------------------------------------------------------Buzzer----------------------------------------------------------------//

    // Sets what phase Bob is in (ascending/descending/landed).
    setPhase(altitude, time);

    // After landing, beep in set intervals.
    Beep();

//---------------------------------------------------------Delay Between Loops----------------------------------------------------------//
    
    // Makes sure there is a delay of at least 250ms between every loop.
    Wait(time);

    yield();
}

/// TODO:
///     Make lib folders into submodules.
///     Prevent SD from overflowing.
///     Create SD support for Ground Station.
///     Calibrate Chip Temperature.