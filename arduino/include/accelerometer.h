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

    double getX();

    double getY();

    double getZ();

    void readTemperature();

    double getTemp();

    void update();

private:
    LIS3DHTR<TwoWire> LIS;
    double _x;
    double _y;
    double _z;
    double _temp;
    unsigned long lastRead;
    // Read interval in milliseconds
    static const unsigned readInterval = 5000;
};

#endif // ACCELEROMETER_H