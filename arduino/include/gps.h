#ifndef GPS_H
#define GPS_H

#include <TinyGPSPlus.h>

/**
 * @class GPS
 * @brief A class to interface with Grove GPS module.
 *
 * @note Ensure that the GPS module is placed in an open area with a clear view of the sky to avoid any signal interference.
 * It needs to find the satellite which can take up to 5 minutes before it starts sending the correct data.
 */
class GPS
{
public:
    /**
     * @brief Constructor for the `GPS` class.
     */
    GPS();

    /**
     * @brief Initializes the GPS module.
     *
     * This function sets up the necessary configurations and prepares the
     * GPS module for data acquisition. It should be called in the main setup()
     * function before any other GPS functions are used.
     */
    void init();

    /**
     * @brief Retrieves the latitude value from the GPS module.
     *
     * @return `double` : The latitude value in decimal degrees. 0.0 if invalid.
     *
     * @note Latitude will be invalid if the GPS does not receive enough correct data.
     * Ensure that the `update()` function is called before reading the latitude value.
     */
    double getLatitude();

    /**
     * @brief Retrieves the longitude value from the GPS module.
     *
     * @return `double` : The longitude value in decimal degrees. 0.0 if invalid.
     *
     * @note Longitude will be invalid if the GPS does not receive enough correct data.
     * Ensure that the `update()` function is called before reading the longitude value.
     */
    double getLongitude();

    /**
     * @brief Updates the GPS instance.
     *
     * This function reads the GPS data from the serial port and updates the
     * internal GPS object with the latest information.
     */
    void update();

private:
    /// @brief The TinyGPSPlus object.
    TinyGPSPlus gps;
};

#endif // GPS_H