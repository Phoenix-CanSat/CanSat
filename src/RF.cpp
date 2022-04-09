#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>

#include "RF.h"
#include "Output.h"

#define RFM_CS 10
#define RFM_RST 3
#define RFM_INT 2

RH_RF95 rf(RFM_CS, RFM_INT);
bool rfInit_ = false;
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
        return rfInit_;
    }

    // Sets RFM frequency to 433.2
    else if (!rf.setFrequency(433.2)) {
        Say("RF freq not set.");
    }
    
    else {
        // Sets the transmitter power output level.
        rf.setTxPower(23, false);

        //Initialization successful.
        rfInit_ = true;
    }

    return rfInit_;
}

// Sends given packet text through RFM to receiver.
bool RFSendData(char packet[]) {

    if (!rfInit_) {
        Say("RF off.");
        return false;
    }
    
    char data[225];
    uint8_t len = snprintf(data, 225, "PHX,%lu,%s", ++packetnumber, packet);
    
    // Send the packet.
    bool sent = rf.send((uint8_t*)data, len);

    // Wait for the packet to be sent.
    rf.waitPacketSent();

    return sent;
}