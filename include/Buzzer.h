#ifndef BUZZER_FUNCTIONS_H
#define BUZZER_FUNCTIONS_H

#include <Arduino.h>

/// Sets what phase Bob is in (ascending/descending/landed).
void setPhase(float alt, uint32_t time);

// After landing, beep in set intervals.
void Beep();

#endif