#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <Wire.h>
#include <LIS3DHTR.h>
#include <math.h>

#define WIRE Wire

class Accelerometer
{
public:
    Accelerometer(bool temp_en);

    void init();

    bool checkConnection();

    bool isAvailable();

    void readAcceleration();

    double getX();

    double getY();

    double getZ();

    /**
     * @brief Compute the roll angle of the accelerometer.
     *
     * Roll angle is the rotation around the X-axis, from the acceleration data.
     *
     * @return `double` : The roll angle in degrees.
     *
     * Roll angle is at 0° when horizontal and at +/- 180° when upside down.
     *
     * Increase when rotating clockwise and decrease counter-clockwise.
     *
     */
    double getRoll();

    double getPitch();

    void readTemperature();

    double getTemp();

    void read();

private:
    bool TEMP_EN;
    LIS3DHTR<TwoWire> LIS;
    double x;
    double y;
    double z;
    double temp;
    unsigned long lastRead;
    // Read interval in milliseconds
    static const unsigned readInterval = 10000;
};

#endif // ACCELEROMETER_H