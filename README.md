# Caterpillar IoT Project

This project aims to monitor and control the real-time position of rolling vehicles within an industrial park, such as those used by companies like Caterpillar.
The system is equipped with RFID sensors to identify and verify the operators of these vehicles in real-time.
Additionally, a gyroscope is used to monitor the status of the vehicles and determine if they need assistance based on their angle of inclination.
By integrating GPS tracking, RFID technology, and gyroscope data, the enterprise gains comprehensive visibility into the location, movement, and status of its fleet on-site.
This enhances operational efficiency, safety, security, and compliance by ensuring that only qualified and authorized personnel operate the vehicles and that any irregularities in vehicle behavior are promptly addressed.

Data is gathered with an Adafruit Feather M0 which transmit them to The Things Network (TTN) using LoRaWAN technology with the help of a gateway supposed to be already installed.

## Features

-   **Real-time GPS Tracking**: Monitor the real-time position of vehicles within the industrial park.
-   **RFID Verification**: Identify and verify the operators of the vehicles using RFID sensors.
-   **Gyroscope Monitoring**: Track the status of vehicles and detect if they need assistance.
-   **Web Interface**: Display vehicle locations on a map and provide detailed information about each vehicle.
-   **Alerts**: Show warning indicators for vehicles that need assistance.

## Technologies Used

-   **Arduino / C++**: Programming language used to develop hardware programming logic.
-   **The Things Network (TTN)**: LoRaWAN network server used to transmit data from the vehicles to the cloud.
-   **LoRaWAN**: For transmitting data to The Things Network (TTN).
-   **MQTT**: For communication between TTN and the backend server.
-   **Node.js**: Backend server to process data and serve the web interface.
-   **Express.js**: Web framework for Node.js used to create the web interface.
-   **Socket.io**: For real-time communication between the backend and frontend.
-   **Leaflet.js**: JavaScript library for interactive maps used to display vehicle locations.
-   **Tailwind CSS**: Utility-first CSS framework used for styling the web interface.

## Setup and Installation

First, clone the repository to your local machine:

```bash
git clone https://github.com/Dori-Tos/catterpillar.git
cd caterpillar
```

### Prerequisites

Install `PlatformIO` extension in `Visual Studio Code` or the `Arduino IDE`.

Ensure you also have `Node.js` installed on your machine.

### Backend Setup

1. Navigate to the `web` folder:

```bash
cd web
```

2. Install the required dependencies:

```bash
npm install
```

3. Create a `.env` file in the `web` folder and add your TTN application credentials:

```sh
TTN_PASSWORD=your_ttn_password
```

4. Start the backend server:

```bash
node app.js
```

5. Open your browser and navigate to `http://localhost:3000` to view the web interface.

### Hardware Setup

#### PlatformIO

1. Open the PlatformIO project catterpillar/arduino (where the `platformio.ini` file is located) with PlatformIO extension in Visual Studio Code.
2. Let PlatformIO install the necessary libraries in `.pio` folder.
3. Connect the Adafruit Feather M0 to your computer using a USB cable.
4. Build and upload the code to the Adafruit Feather M0.
5. Open the Serial Monitor to view the output.

#### Arduino IDE

Arduino IDE requires you to have a folder containing a `.ino` file with the same name as the folder. Every other `.cpp` and `.h` files should be in that same folder.

1. Create a new folder with the name of your choice in the `arduino` folder:

```bash
mkdir arduino/main
```

2. Copy the contents of the `arduino/src` and `arduino/include` folders to the newly created folder.

```bash
cp -r arduino/src arduino/main
cp -r arduino/include arduino/main
```

3. Rename the `main.cpp` file to `main.ino`.

```bash
mv arduino/main/main.cpp arduino/main/main.ino
```

4. Open the `main.ino` file with the Arduino IDE.

5. Make sure you have `Adafruit SAMD Boards` and the following Arduino libraries installed:

    - `mcci-catena/MCCI LoRaWAN LMIC library@^4.1.1`
    - `seeed-studio/Grove-3-Axis-Digital-Accelerometer-2g-to-16g-LIS3DHTR@^1.2.4`
    - `mikalhart/TinyGPSPlus@^1.1.0`
    - `miguelbalboa/MFRC522@^1.4.11`

6. Connect the Adafruit Feather M0 to your computer using a USB cable.

7. Build and upload the code to the Adafruit Feather M0.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
