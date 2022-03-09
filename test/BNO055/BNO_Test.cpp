#include <Arduino.h>

#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup(void) {
    while(!Serial);
    Serial.begin(9600);
    Serial.println("Orientation Sensor Test");
    Serial.println("");

    /* Initialise the sensor */
    if (!bno.begin()) {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while (1);
    }

    delay(1000);

    bno.setExtCrystalUse(true);
}

void loop(void) {
    
}