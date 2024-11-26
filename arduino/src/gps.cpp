#include "gps.h"

GPS::GPS() {}

// TODO: Use serial communication to get GPS data

void GPS::init()
{
    // Serial.begin(9600);
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
    // while (Serial.available() > 0) {
    //     gps.encode(Serial.read());
    // }
}