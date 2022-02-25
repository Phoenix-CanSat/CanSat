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
uint32_t lastPacketSentTime = 0;
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
        Say("Could not initialize the RF.");
    }

    // Sets RFM frequency to 433.2
    else if (!rf.setFrequency(433.2)) {
        Say("Could not set the RF frequency to 433.2 Hz.");
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
bool RFSendData(char packet[], uint8_t len) {

    if (!rfInit_) {
        Say("RF is not initialized.");
        return false;
    }

    // Makes sure there is a delay of 50ms between every packet.
    uint32_t interval = millis() - lastPacketSentTime;
    if (interval < 50) {
        delay(50 - interval);
    }

    // Send the packet.
    bool sent = rf.send((uint8_t*)packet, len);
    // Wait for the packet to be sent.
    rf.waitPacketSent();

    // Appropriate message if failed to send packet.
    if (!sent) {
        Say("RF could not send packet to receiver.");
    }

    // Store time since boot the last packet was sent.
    lastPacketSentTime = millis();

    return sent;
}