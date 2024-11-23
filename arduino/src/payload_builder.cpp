#include "payload_builder.h"

void PayloadBuilder::buildAccelerationPayload(uint8_t *payload, Accelerometer accelerometer)
{
    // .00 resolution -> *100
    int16_t roll = (int16_t)(accelerometer.getRoll() * 100);
    int16_t pitch = (int16_t)(accelerometer.getPitch() * 100);
    payload[17] = (roll >> 8) & 0xFF;
    payload[18] = roll & 0xFF;
    payload[19] = (pitch >> 8) & 0xFF;
    payload[20] = pitch & 0xFF;
}