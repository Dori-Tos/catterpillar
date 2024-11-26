#ifndef GPS_H
#define GPS_H

#include <TinyGPSPlus.h>

class GPS
{
public:
    GPS();

    void init();

    bool isAvailable();

    double getLatitude();

    double getLongitude();

    void update();

private:
    TinyGPSPlus gps;
};

#endif // GPS_H