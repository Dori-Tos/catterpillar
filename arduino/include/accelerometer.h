#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <Wire.h>
#include <LIS3DHTR.h>

#define WIRE Wire

class Accelerometer
{
public:
    Accelerometer();

    void init();

    bool checkConnection();

    bool isAvailable();

    void readAcceleration(float &x, float &y, float &z);

    void readTemperature(float &temp);

    void loopAccelerometer();

private:
    LIS3DHTR<TwoWire> LIS;
};

#endif // ACCELEROMETER_H