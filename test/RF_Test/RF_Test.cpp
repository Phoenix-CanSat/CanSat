#include <Arduino.h>
#include <Wire.h>

#include <SPI.h>
#include <RH_RF95.h>

#include "RF.h"

void setup() {
    Serial.begin(9600);

    delay(4000);

    if (RFInit()) {
        Serial.println("RF: State: Init");
    }
    else {
        Serial.println("RF: State: Not Init");
    }

    delay(1000);
}

int16_t packetnum = 0;

void loop() {
    char testdata[15] = "Hello World! #";
    itoa(packetnum++, testdata + 13, 10);

    Serial.println("RF sending...");
    if (RFSendData(testdata, 15)) {
        Serial.println("RF: Send: Succeeded");
    } else {
        Serial.println("RF: Send: Failed");
    }
    Serial.println("RF: Send: Finished");

    delay(2000);
}