#ifndef _TONE32_h
#define _TONE32_h

#define TONE_CHANNEL   15
#define BUZZER_PIN     18
#define BUZZER_CHANNEL 0
#define BUZZER_FREQ    1000

#include "Arduino.h"

void tone(uint8_t pin, unsigned int frequency, uint8_t channel = TONE_CHANNEL);
void noTone(uint8_t pin, uint8_t channel = TONE_CHANNEL);
#endif