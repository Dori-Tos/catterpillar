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

    void readAcceleration();

    float getX();

    float getY();

    float getZ();

    void readTemperature();

    float getTemp();

    void update();

private:
    LIS3DHTR<TwoWire> LIS;
    float _x;
    float _y;
    float _z;
    float _temp;
    unsigned long lastRead;
    // Read interval in milliseconds
    static const unsigned readInterval = 5000;
};

#endif // ACCELEROMETER_H