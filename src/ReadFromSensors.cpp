#include <Arduino.h>
#include <Wire.h>
#include <utility/imumaths.h>
#include <Adafruit_Sensor.h>

#include <Adafruit_BME280.h>
#include <Adafruit_GPS.h>
#include <Adafruit_BNO055.h>

#include "ReadFromSensors.h"
#include "Output.h"

Adafruit_BME280 BME;
Adafruit_GPS GPS(&Serial7);
Adafruit_BNO055 BNO;

bool bmeinit = false;
bool gpsinit = false;
bool bnoinit = false;

#define PressureSamples 20
float GroundPressure;

bool GPSWasRead = false;
char c;

imu::Vector<3> magnetic;
imu::Vector<3> gravity;
uint8_t BNOMagRead = 0;
uint8_t BNOGravRead = 0;

//-------------------------------------------------------Miscellaneous Functions--------------------------------------------------------//

// Read pressure multiple times at ground level to calculate the average pressure at ground level.
void CalculateGroundPressure() {
    for (int i = 0; i < PressureSamples; i++) {
        GroundPressure += BME.readPressure() / 100.0F;
        delay(50);
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
        GPS.sendCommand("$PGCMD,33,0*6D");              // Turns off antena update data.
        GPS.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);     // Sets update rate to 10Hz.
        GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);   // Send only RMC and GGA sentences.
        delay(1000);
        gpsinit = true;
    }
    return gpsinit;
}

// Initializes BNO055
bool BNOInit() {
    if (BNO.begin()) {
        bnoinit = true;
    }
    return bnoinit;
}

//-----------------------------------------------------Read From Sensors Functions------------------------------------------------------//

// Temperature in Celsius (°C).
float GetTemperature() {
    // No value if BME280 isn't initialized.
    if (!bmeinit) {
        return 0;
    } else {
        return BME.readTemperature();
    }
}

// Pressure in HectoPascal/Millibar (hPa/mbar).
float GetPressure() {
    // No value if BME280 isn't initialized.
    if (!bmeinit) {
        return 0;
    } else {
        return BME.readPressure() / 100.0F;
    }
}

// Latitude in Degrees (°).
float GetLatitude() {
    // No value if GPS isn't initialized or doesn't get a fix.
    if (!gpsinit || !GPS.fix) {
        return 0;
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
        return 0;
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
        return 0;
    } else {
        return BME.readAltitude(GroundPressure);
    }
}

// Humidity in Percentage (%).
float GetHumidity() {
    // No value if BME280 isn't initialized.
    if (!bmeinit) {
        return 0;
    } else {
        return BME.readHumidity();
    }
}

// Magnetic Field Strength in micro Teslas (uT).
float GetMagnetic(uint8_t axis) {
    // No value if BNO055 isn't initialized.
    if (!bnoinit) {
        return 0;
    } else {
        // Checks how many values have been read. Can read up to 3, for 3 axis.
        switch (BNOMagRead) {
            // If no values have been read, get new values from vector.
            case 0:
                magnetic = BNO.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
                BNOMagRead += 1;
                break;
            // If 3 values have been read, reset counter.
            case 3:
                BNOMagRead = 0;
                break;
            // If 1 or 2 values have been read, update counter.
            default:
                BNOMagRead += 1;
        }
        // Get value of respective axis.
        switch (axis) {
            case X:
                return magnetic.x();
            case Y:
                return magnetic.y();
            case Z:
                return magnetic.z();
            default:
                return 0;
        }
    }
}

// Gravitational Field Strength in Meters per Second squared (m/s²).
float GetGravity(uint8_t axis) {
    // No value if BNO055 isn't initialized.
    if (!bnoinit) {
        return 0;
    } else {
        // Checks how many values have been read. Can read up to 3, for 3 axis.
        switch (BNOGravRead) {
            // If no values have been read, get new values from vector.
            case 0:
                gravity = BNO.getVector(Adafruit_BNO055::VECTOR_GRAVITY);
                BNOGravRead += 1;
                break;
            // If 3 values have been read, reset counter.
            case 3:
                BNOGravRead = 0;
                break;
            // If 1 or 2 values have been read, update counter.
            default:
                BNOGravRead += 1;
        }
        // Get value of respective axis.
        switch (axis) {
            case X:
                return gravity.x();
            case Y:
                return gravity.y();
            case Z:
                return gravity.z();
            default:
                return 0;
        }
    }
}