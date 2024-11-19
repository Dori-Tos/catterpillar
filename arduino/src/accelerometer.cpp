#include "accelerometer.h"

Accelerometer::Accelerometer()
{
    LIS = LIS3DHTR<TwoWire>();
    _x, _y, _z, _temp = float();
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
    _x = LIS.getAccelerationX();
    _y = LIS.getAccelerationY();
    _z = LIS.getAccelerationZ();
}

float Accelerometer::getX()
{
    return _x;
}

float Accelerometer::getY()
{
    return _y;
}

float Accelerometer::getZ()
{
    return _z;
}

void Accelerometer::readTemperature()
{
    _temp = LIS.getTemperature();
}

float Accelerometer::getTemp()
{
    return _temp;
}

void Accelerometer::loopAccelerometer()
{
    if (!checkConnection())
    {
        Serial.println("LIS3DHTR didn't connect.");
        while (1)
            ;
        return;
    }
    readAcceleration();
    readTemperature();
    Serial.printf("x: %.2f  y: %.2f  z: %.2f\n", _x, _y, _z);
    Serial.printf("temp: %.2f\n", _temp);
    delay(500);
}