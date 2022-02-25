#ifndef RF_FUNCTIONS_H
#define RF_FUNCTIONS_H

#include <Arduino.h>

/// Initializes RFM9X.
/// Sets frequency and transmition power.
/// @return true if initialization is successful and frequency was set.
/// @return false if initialization is unsuccessful or frequency could not be set.
bool RFInit();

/// Sends given text/packet through RFM to receiver with a minimum delay of 50ms between each packet.
/// @return true if packet is sent.
/// @return false if packet could not be sent or RFM wasn't initialized.
bool RFSendData(char packet[], uint8_t len);

#endif