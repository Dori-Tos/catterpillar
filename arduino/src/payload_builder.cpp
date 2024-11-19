#include "payload_builder.h"

void PayloadBuilder::buildAccelerationPayload(uint8_t *payload, Accelerometer accelerometer)
{
    // .00 resolution -> *100
    int16_t x = (int16_t)(accelerometer.getX() * 100);
    int16_t y = (int16_t)(accelerometer.getY() * 100);
    int16_t z = (int16_t)(accelerometer.getZ() * 100);
    payload[17] = (x >> 8) & 0xFF;
    payload[18] = x & 0xFF;
    payload[19] = (y >> 8) & 0xFF;
    payload[20] = y & 0xFF;
    payload[21] = (z >> 8) & 0xFF;
    payload[22] = z & 0xFF;
}