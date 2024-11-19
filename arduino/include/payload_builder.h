#ifndef PAYLOAD_BUILDER_H
#define PAYLOAD_BUILDER_H

#include <stdint.h>
#include "accelerometer.h"

class PayloadBuilder
{
public:
    static void buildAccelerationPayload(uint8_t *payload, Accelerometer accelerometer);
    // Add more build functions for other sensors as needed
};

#endif // PAYLOAD_BUILDER_H