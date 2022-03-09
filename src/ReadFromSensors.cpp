#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>

#include <Adafruit_BME280.h>
#include <Adafruit_GPS.h>

#include "ReadFromSensors.h"
#include "Output.h"

Adafruit_BME280 BME;
Adafruit_GPS GPS(&Serial7);

bool bmeinit = false;
bool gpsinit = false;

#define PressureSamples 20
#define PressureSampleDelay 50
float GroundPressure;

bool GPSWasRead = false;
char c;

//-------------------------------------------------------Miscellaneous Functions--------------------------------------------------------//

// Read pressure multiple times at ground level to calculate the average pressure at ground level.
void CalculateGroundPressure() {
    for (int i = 0; i < PressureSamples; i++) {
        GroundPressure += BME.readPressure() / 100.0F;
        delay(PressureSampleDelay);
    }
    GroundPressure /= PressureSamples;
}

// Reads 3 NMEA sentences from GPS to avoid corrupt data, then reads 2 more and parses them.
void readGPS() {
    for (int i = 0; i < 5; i++) {
        while (!GPS.newNMEAreceived()) {
            c = GPS.read();
        }
        GPS.parse(GPS.lastNMEA());
    }
    GPSWasRead = true;
}

//-------------------------------------------------------Initialization Functions-------------------------------------------------------//

//  Initializes BME280
bool BMEInit() {
    if (BME.begin()) {
        CalculateGroundPressure();
        bmeinit = true;
    }
    return bmeinit;
}

// Initializes GPS
bool GPSInit() {
    if (GPS.begin(9600)) {
        GPS.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);     // Sets update rate to 10Hz.
        GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);   // Send only RMC and GGA sentences.
        delay(1000);
        gpsinit = true;
    }
    return gpsinit;
}

//-----------------------------------------------------Read From Sensors Functions------------------------------------------------------//

// Temperature in Celsius (°C).
float GetTemperature() {
    // No value if BME280 isn't initialized.
    if (!bmeinit) {
        return (float)NAN;
    } else {
        return BME.readTemperature();
    }
}

// Pressure in HectoPascal/Millibar (hPa/mbar).
float GetPressure() {
    // No value if BME280 isn't initialized.
    if (!bmeinit) {
        return (float)NAN;
    } else {
        return BME.readPressure() / 100.0F;
    }
}

// Latitude in Degrees (°).
float GetLatitude() {
    // No value if GPS isn't initialized or doesn't get a fix.
    if (!gpsinit || !GPS.fix) {
        return (float)NAN;
    } else {
            // Parse data from NMEA sentence. If required data is already parsed, enable parsing of new data for later.
            if (GPSWasRead == false) {
                readGPS();
            } else {
                GPSWasRead = false;
            }
            // Latitude will have a negative sign for Southern Hemisphere.
            return GPS.latitude * (1 - 2 * (GPS.lat == 'S'));
        }
    }

// Longitude in Degrees (°).
float GetLongitude() {
    // No value if GPS isn't initialized or doesn't get a fix.
    if (!gpsinit || !GPS.fix) {
        return (float)NAN;
    } else {
        // Parse data from NMEA sentence. If required data is already parsed, enable parsing of new data for later.
        if (GPSWasRead == false) {
            readGPS();
        } else {
            GPSWasRead = false;
        }
        // Latitude will have a negative sign for Western Hemisphere.
        return GPS.longitude * (1-2*(GPS.lon=='W'));
    }
}

// Altitude from current ground level in Meters (m).
float GetAltitude() {
    // No value if BME280 isn't initialized.
    if (!bmeinit) {
        return (float)NAN;
    } else {
        return BME.readAltitude(GroundPressure);
    }
}

// Humidity in Percentage (%).
float GetHumidity() {
    // No value if BME280 isn't initialized.
    if (!bmeinit) {
        return (float)NAN;
    } else {
        return BME.readHumidity();
    }
}