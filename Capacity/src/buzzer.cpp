#include "buzzer.h"

void tone(uint8_t pin, unsigned int frequency, uint8_t channel)
{
    if (ledcRead(channel))
    {
        log_e("Tone channel %d is already in use", channel);
        return;
    }
    ledcAttachPin(pin, channel);
    ledcWriteTone(channel, frequency);
}

void noTone(uint8_t pin, uint8_t channel)
{
    ledcDetachPin(pin);
    ledcWrite(channel, 0);
}