#include "accelerometer.h"

Accelerometer::Accelerometer()
{
    LIS = LIS3DHTR<TwoWire>();
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

void Accelerometer::readAcceleration(float &x, float &y, float &z)
{
    // Static in x direction : 1g (x), 0g (y), 0g (z)
    // Static in y direction : 0g (x), 1g (y), 0g (z)
    // Static in z direction : 0g (x), 0g (y), 1g (z)
    x = LIS.getAccelerationX();
    y = LIS.getAccelerationY();
    z = LIS.getAccelerationZ();
}

void Accelerometer::readTemperature(float &temp)
{
    temp = LIS.getTemperature();
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
    float x, y, z, temp;
    readAcceleration(x, y, z);
    readTemperature(temp);
    Serial.print("x:");
    Serial.print(x);
    Serial.print("  ");
    Serial.print("y:");
    Serial.print(y);
    Serial.print("  ");
    Serial.print("z:");
    Serial.println(z);
    Serial.print("temp:");
    Serial.println(temp);
    delay(500);
}