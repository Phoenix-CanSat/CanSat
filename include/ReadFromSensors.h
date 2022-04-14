#ifndef READFROMSENSORS_FUNCTIONS_H
#define READFROMSENSORS_FUNCTIONS_H

#include <Arduino.h>

//-------------------------------------------------------Initialization Functions-------------------------------------------------------//

/// Initializes respective sensor.
/// @return true if initialization is successful.
/// @return false if initialization is unsuccessful.
bool BMEInit();
bool GPSInit();

//-----------------------------------------------------Read From Sensors Functions------------------------------------------------------//

/// Reads required value from respective sensor.
/// @return value in float.
float GetTemperature();                 // Temperature in Celsius (°C).
float GetPressure();                    // Pressure in HectoPascal/Millibar (hPa/mbar).
float GetLatitude();                    // Latitude in Degrees (°).
float GetLongitude();                   // Longitude in Degrees (°).
float GetAltitude();                    // Altitude from current ground level in Meters (m).
float GetHumidity();                    // Humidity in Percentage (%).
float GetInternalTemperature();         // Internal Temperature in Celsius (°C).

#endif