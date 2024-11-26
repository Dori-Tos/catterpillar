#include "rfid.h"

// Serial2 definition ////////////////////////////////

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
    cardID = 0;
}

void RFID::init()
{
    Serial2.begin(115200);

    // Assign pins 10 & 12 SERCOM functionality
    pinPeripheral(10, PIO_SERCOM);
    pinPeripheral(12, PIO_SERCOM);
}

bool RFID::isCardPresent()
{
    return cardPresent;
}

uint32_t RFID::getCardID()
{
    return cardID;
}

void RFID::listen()
{
    // TODO: Listen the serial port connected to Arduino for RFID card ID
}