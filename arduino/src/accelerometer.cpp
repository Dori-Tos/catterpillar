#include "accelerometer.h"

Accelerometer::Accelerometer()
{
    lastRead = millis();
    LIS = LIS3DHTR<TwoWire>();
    x = 0.0;
    y = 0.0;
    z = 0.0;
    temp = 0.0;
}

void Accelerometer::init()
{
    LIS.begin(WIRE, 0x19); // IIC init
    LIS.openTemp();        // If ADC3 is used, the temperature detection needs to be turned off.
    delay(100);
    LIS.setFullScaleRange(LIS3DHTR_RANGE_2G);
    LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);
}

bool Accelerometer::checkConnection()
{
    return LIS;
}

bool Accelerometer::isAvailable()
{
    return LIS.available();
}

void Accelerometer::readAcceleration()
{
    // Static in x direction : 1g (x), 0g (y), 0g (z)
    // Static in y direction : 0g (x), 1g (y), 0g (z)
    // Static in z direction : 0g (x), 0g (y), 1g (z)
    x = LIS.getAccelerationX();
    y = LIS.getAccelerationY();
    z = LIS.getAccelerationZ();
}

double Accelerometer::getX()
{
    return x;
}

double Accelerometer::getY()
{
    return y;
}

double Accelerometer::getZ()
{
    return z;
}

double Accelerometer::getRoll()
{
    return atan2(y, z) * 180 / M_PI;
}

double Accelerometer::getPitch()
{
    return atan2(-x, sqrt(y * y + z * z)) * 180 / M_PI;
}

void Accelerometer::readTemperature()
{
    temp = LIS.getTemperature();
}

double Accelerometer::getTemp()
{
    return temp;
}

void Accelerometer::read()
{
    if (millis() - lastRead >= readInterval)
    {
        if (isAvailable())
        {
            readAcceleration();
            readTemperature();
            // Serial.printf("x: %.2f  y: %.2f  z: %.2f\n", _x, _y, _z);
            // Serial.printf("temp: %.2f\n", _temp);
            lastRead = millis();
        }
        else
        {
            Serial.println("LIS3DHTR not available");
        }
    }
}