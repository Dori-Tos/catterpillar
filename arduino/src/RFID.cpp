#include "rfid.h"

// RX: pin 12 (S1.3), TX: pin 10 (S1.2)
Uart Serial2(&sercom1, 12, 10, SERCOM_RX_PAD_3, UART_TX_PAD_2);

void SERCOM1_Handler()
{
    Serial2.IrqHandler();
}
/////////////////////////////////////////////////////

RFID::RFID()
{
    cardPresent = false;
    msg = "";
}

void RFID::init()
{
    Serial2.begin(9600);

    // Assign pins 10 & 12 SERCOM functionality
    pinPeripheral(10, PIO_SERCOM);
    pinPeripheral(12, PIO_SERCOM);
}

int RFID::getCardID(int i)
{
    return cardID[i];
}

void RFID::listen()
{
    if (Serial2.available() > 0)
    {
        String message = Serial2.readStringUntil('\n'); 
        msg = trimString(message);
        if (msg.length() > 0) {
            ConvertCardId();
            cardPresent = true;
        } else {
            cardPresent = false;
        }
    } 
}

void RFID::ConvertCardId()
{
    // Split the message into hex values
    String hexValues[20];
    size_t count = 0;
    splitString(msg, ' ', hexValues, count);

    // Convert and store each value
    for (size_t i = 0; i < count; i++) {
        int value = fromHexToInt(hexValues[i]);
        if (value != -1) {
            cardID[i] = value;
        }
    }
}   

int RFID::fromHexToInt(const String& hexString)
{
    for (size_t i = 0; i < hexString.length(); i++) {
        if (!isxdigit(hexString[i])) {
            Serial.print("Invalid character in hexadecimal string: ");
            Serial.println(hexString);  // Print the full string
            return -1;
        }
    }
    return strtol(hexString.c_str(), nullptr, 16);
}

void RFID::splitString(const String& str, char delimiter, String results[], size_t& count)
{
    count = 0;
    size_t start = 0;
    size_t end = str.indexOf(delimiter);
    while (end != -1) {
        if (end > start) {
            results[count++] = str.substring(start, end);
        }
        start = end + 1;
        end = str.indexOf(delimiter, start);
    }
    if (start < str.length()) {
        results[count++] = str.substring(start);
    }
}

String RFID::trimString(const String& str)
{
    int start = 0;
    while (start < str.length() && isspace(str[start])) start++;
    int end = str.length() - 1;
    while (end >= start && isspace(str[end])) end--;
    return (start <= end) ? str.substring(start, end + 1) : "";
}
