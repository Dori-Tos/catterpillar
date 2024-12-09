#include "accelerometer.h"

Accelerometer::Accelerometer(bool temp_en)
{
    TEMP_EN = temp_en;
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
    if (TEMP_EN)
        LIS.openTemp(); // If ADC3 is used, the temperature detection needs to be turned off.
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
    double sumX = 0.0;
    double sumY = 0.0;
    double sumZ = 0.0;
    const int numReadings = 10;

    for (int i = 0; i < numReadings; i++)
    {
        sumX += (double)LIS.getAccelerationX();
        sumY += (double)LIS.getAccelerationY();
        sumZ += (double)LIS.getAccelerationZ();
        delay(10); // Small delay between readings
    }

    x = sumX / (double)numReadings;
    y = sumY / (double)numReadings;
    z = sumZ / (double)numReadings;
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
    return atan2(y, z) * 180.0 / M_PI;
}

double Accelerometer::getPitch()
{
    double pitch = atan2(-x, sqrt(y * y + z * z)) * 180.0 / M_PI;
    // If the z value is negative, the pitch angle should be inverted due to trigonometric signs.
    if (z < 0)
    {
        if (x < 0)
        {
            pitch = 180 - pitch;
        }
        else
        {
            pitch = -180 - pitch;
        }
    }
    return pitch;
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
            if (TEMP_EN)
            {
                readTemperature();
            }
            lastRead = millis();
        }
        else
        {
            Serial.println("LIS3DHTR not available");
        }
    }
}