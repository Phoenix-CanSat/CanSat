#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>

#include "RF.h"
#include "Output.h"

#define RFM_CS 10
#define RFM_RST 3
#define RFM_INT 2

RH_RF95 rf(RFM_CS, RFM_INT);
bool rf_init = false;
uint32_t packetnumber = 0;

// Initializes RFM9X.
bool RFInit() {

    // Resets RFM.
    pinMode(RFM_RST, OUTPUT);
    digitalWrite(RFM_RST, LOW);
    delay(100);
    digitalWrite(RFM_RST, HIGH);
    delay(100);

    // If failed, send appropriate message.
    if (!rf.init()) {
        return rf_init;
    }

    // Sets RFM frequency to 433.2
    else if (!rf.setFrequency(433.2)) {
        Say("RF freq not set.");
    }
    
    else {
        // Sets the transmitter power output level.
        rf.setTxPower(23, false);

        //Initialization successful.
        rf_init = true;
    }

    return rf_init;
}

// Sends given packet text through RFM to receiver.
bool RFSendData(const char text[], bool info = false) {

    if (rf_init) {

        // Add packet identifier and packet counter to packet.
        char packet[225];
        uint8_t len;
        if (!info) {
            len = snprintf(packet, 225, "PHX,%lu,%s", ++packetnumber, text);
        } else {
            len = snprintf(packet, 225, "PHXINFO,%s", text);
        }

        // Send the packet.
        bool sent = rf.send((uint8_t*)packet, len);

        // Wait for the packet to be sent.
        rf.waitPacketSent();

        return sent;
    }

    return false;
}