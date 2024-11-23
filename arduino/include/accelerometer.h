#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <Wire.h>
#include <LIS3DHTR.h>
#include <math.h>

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

    double getRoll();

    double getPitch();

    void readTemperature();

    double getTemp();

    void read();

private:
    LIS3DHTR<TwoWire> LIS;
    double x;
    double y;
    double z;
    double temp;
    unsigned long lastRead;
    // Read interval in milliseconds
    static const unsigned readInterval = 5000;
};

#endif // ACCELEROMETER_H