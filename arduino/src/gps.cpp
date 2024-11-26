#include "gps.h"

GPS::GPS() {}

void GPS::init()
{
    // Serial1 -> RX: pin 0 (S0.3), TX: pin 1 (S0.2)
    Serial1.begin(9600);
}

bool GPS::isAvailable()
{
    return gps.location.isValid();
}

double GPS::getLatitude()
{
    return gps.location.lat();
}

double GPS::getLongitude()
{
    return gps.location.lng();
}

void GPS::update()
{
    // TODO: Use serial communication to get GPS data

    // while (Serial.available() > 0) {
    //     gps.encode(Serial.read());
    // }
}