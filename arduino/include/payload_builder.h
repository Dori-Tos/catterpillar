#ifndef PAYLOAD_BUILDER_H
#define PAYLOAD_BUILDER_H

#include <stdint.h>
#include "gps.h"
#include "accelerometer.h"
#include "rfid.h"

class PayloadBuilder
{
public:
    static void buildVehiclePayload(uint8_t *payload, uint16_t vehicleID);

    static void buildGPSPayload(uint8_t *payload, GPS gps);

    static void buildAccelerometerPayload(uint8_t *payload, Accelerometer accelerometer);

    static void buildRFIDPayload(uint8_t *payload, RFID rfid);
};

#endif // PAYLOAD_BUILDER_H