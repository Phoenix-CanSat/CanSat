#ifndef READFROMSENSORS_FUNCTIONS_H
#define READFROMSENSORS_FUNCTIONS_H

#include <Arduino.h>

// Initialization Time
#define INITTIME 10000

#define X 0
#define Y 1
#define Z 2

//-------------------------------------------------------Initialization Functions-------------------------------------------------------//

/// Initializes respective sensor.
/// @return true if initialization is successful.
/// @return false if initialization is unsuccessful.
bool BMEInit();
bool GPSInit();
bool BNOInit();

//-----------------------------------------------------Read From Sensors Functions------------------------------------------------------//

/// Reads time since initialization.
/// @return value in unsigned integer.
unsigned int GetTime();

/// Reads required value from respective sensor.
/// @return value in float
float GetTemperature();                 // Temperature in Celsius (°C).
float GetPressure();                    // Pressure in HectoPascal/Millibar (hPa/mbar).
float GetLatitude();                    // Latitude in Degrees (°).
float GetLongitude();                   // Longitude in Degrees (°).
float GetAltitude();                    // Altitude from current ground level in Meters (m).
float GetHumidity();                    // Humidity in Percentage (%).
float GetMagnetic(uint8_t axis);        // Magnetic Field Strength in micro Teslas (uT).
float GetGravity(uint8_t axis);         // Gravitational Field Strength in Meters per Second squared (m/s²).

#endif