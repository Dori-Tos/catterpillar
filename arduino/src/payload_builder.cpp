#include "payload_builder.h"

void PayloadBuilder::buildVehiclePayload(uint8_t *payload, uint16_t vehicleID)
{
    payload[0] = (vehicleID >> 8) & 0xFF;
    payload[1] = vehicleID & 0xFF;
}

void PayloadBuilder::buildGPSPayload(uint8_t *payload, GPS gps)
{
    // .000001 resolution -> *100000
    int32_t latitude = (int32_t)(gps.getLatitude() * 100000);
    // int32_t latitude = (int32_t)(50.84986 * 100000); //! Temporary test value
    int32_t longitude = (int32_t)(gps.getLongitude() * 100000);
    // int32_t longitude = (int32_t)(4.45389 * 100000); //! Temporary test value
    payload[2] = (latitude >> 20) & 0xFF;
    payload[3] = (latitude >> 12) & 0xFF;
    payload[4] = (latitude >> 4) & 0xFF;
    payload[5] = ((latitude & 0xF) << 4) | ((longitude >> 24) & 0xF); // 4 bits of latitude and 4 bits of longitude
    payload[6] = (longitude >> 16) & 0xFF;
    payload[7] = (longitude >> 8) & 0xFF;
    payload[8] = longitude & 0xFF;
}

void PayloadBuilder::buildAccelerometerPayload(uint8_t *payload, Accelerometer accelerometer)
{
    // .00 resolution -> *100
    int16_t roll = (int16_t)(accelerometer.getRoll() * 100);
    int16_t pitch = (int16_t)(accelerometer.getPitch() * 100);
    payload[9] = (roll >> 8) & 0xFF;
    payload[10] = roll & 0xFF;
    payload[11] = (pitch >> 8) & 0xFF;
    payload[12] = pitch & 0xFF;
}

void PayloadBuilder::buildRFIDPayload(uint8_t *payload, RFID rfid)
{
    payload[13] = rfid.getCardID(0);
    payload[14] = rfid.getCardID(1);
    payload[15] = rfid.getCardID(2);
    payload[16] = rfid.getCardID(3);
}