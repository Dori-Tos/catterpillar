#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <Wire.h>
#include <LIS3DHTR.h>
#include <math.h>

#define WIRE Wire

/**
 * @class Accelerometer
 * @brief A class to interface with a LIS3DHTR accelerometer sensor.
 *
 * @param temp_en A boolean flag to enable or disable temperature measurement.
 */
class Accelerometer
{
public:
    /**
     * @brief Constructor for the `Accelerometer` class.
     *
     * @param temp_en A boolean flag to enable or disable temperature measurement.
     */
    Accelerometer(bool temp_en);

    /**
     * @brief Initializes the accelerometer.
     *
     * This function sets up the necessary configurations and prepares the
     * accelerometer for data acquisition. It should be called before any
     * other accelerometer functions are used.
     */
    void init();

    /**
     * @brief Checks the connection status of the accelerometer.
     *
     * This function verifies whether the accelerometer is properly connected
     * and communicating with the microcontroller.
     *
     * @return `true` if the connection is successful, `false` otherwise.
     */
    bool checkConnection();

    /**
     * @brief Checks if the accelerometer is available for use.
     *
     * This function determines whether the accelerometer is currently available and ready for data acquisition.
     *
     * @return `true` if the accelerometer is available, `false` otherwise.
     */
    bool isAvailable();

    /**
     * @brief Reads the current acceleration values from the accelerometer.
     *
     * 3D acceleration values are read along the X, Y, and Z axes.
     * They are proportional to the acceleration in g-force units:
     *
     * - Static in x direction : 1g (x), 0g (y), 0g (z)
     *
     * - Static in y direction : 0g (x), 1g (y), 0g (z)
     *
     * - Static in z direction : 0g (x), 0g (y), 1g (z)
     *
     * @note Ensure that the accelerometer is properly initialized before
     * calling this function.
     */
    void readAcceleration();

    /**
     * @brief Retrieves the X-axis acceleration value.
     *
     * @return `double` The acceleration value in g along the X-axis.
     *
     * @note Ensure that the accelerometer has been read at least 1 time before calling this method.
     */
    double getX();

    /**
     * @brief Retrieves the Y-axis acceleration value.
     *
     * @return `double` The acceleration value in g along the Y-axis.
     *
     * @note Ensure that the accelerometer has been read at least 1 time before calling this method.
     */
    double getY();

    /**
     * @brief Retrieves the Z-axis acceleration value.
     *
     * @return `double` The acceleration value in g along the Z-axis.
     *
     * @note Ensure that the accelerometer has been read at least 1 time before calling this method.
     */
    double getZ();

    /**
     * @brief Compute the roll angle of the accelerometer.
     *
     * Roll angle is the rotation around the X-axis.
     *
     * @return `double` : The roll angle in degrees.
     *
     * Roll angle is at 0° when horizontal and at +/- 180° when upside down.
     *
     * Increase when rotating clockwise and decrease counter-clockwise.
     *
     */
    double getRoll();

    /**
     * @brief Compute the pitch angle of the accelerometer.
     *
     * Pitch angle is the rotation around the Y-axis.
     *
     * @return `double` : The pitch angle in degrees.
     *
     * Pitch angle is at 0° when horizontal and at +/- 180° when upside down.
     *
     * Increase when rotating clockwise and decrease counter-clockwise.
     *
     */
    double getPitch();

    /**
     * @brief Reads the temperature from the accelerometer sensor.
     *
     * Temperature value is meant to be used for various purposes such as monitoring the
     * operating environment or compensating for temperature-related variations in sensor readings.
     */
    void readTemperature();

    /**
     * @brief Retrieves the temperature value from the accelerometer sensor.
     *
     * @return `double` : The temperature value in degrees Celsius.
     */
    double getTemp();

    /**
     * @brief Reads all data from the accelerometer.
     *
     * This function uses every read function in the class to get all the data from the accelerometer.
     *
     * Some can be disabled when instantiating the class.
     */
    void read();

private:
    /// @brief A boolean flag to enable or disable temperature measurement.
    bool TEMP_EN;
    /// @brief The LIS3DHTR accelerometer object.
    LIS3DHTR<TwoWire> LIS;
    /// @brief X-axis acceleration value.
    double x;
    /// @brief Y-axis acceleration value.
    double y;
    /// @brief Z-axis acceleration value.
    double z;
    /// @brief Temperature value.
    double temp;
    /// @brief Last time the accelerometer was read in milliseconds.
    unsigned long lastRead;
    /// @brief Time interval between accelerometer reads in milliseconds.
    static const unsigned readInterval = 30000;
};

#endif // ACCELEROMETER_H