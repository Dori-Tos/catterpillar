#include "gps.h"

GPS::GPS() {}

void GPS::init()
{
    // Serial1 -> RX: pin 0 (S0.3), TX: pin 1 (S0.2)
    Serial1.begin(9600);
}

double GPS::getLatitude()
{
    if (gps.location.isValid())
    {
        return gps.location.lat();
    }
    return 0.0;
}

double GPS::getLongitude()
{
    if (gps.location.isValid())
    {
        return gps.location.lng();
    }
    return 0.0;
}

void GPS::update()
{
    // If any characters have arrived from the GPS,
    // send them to the TinyGPS object
    while (Serial1.available() > 0)
    {
        gps.encode(Serial1.read());
    }
}