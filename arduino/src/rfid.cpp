#include "rfid.h"

RFID::RFID()
{
    cardPresent = false;
    cardID = 0;
}

void RFID::init()
{
    // TODO: Initialize serial communication with Arduino
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
    // TODO: Listen the serial port for RFID card ID
}