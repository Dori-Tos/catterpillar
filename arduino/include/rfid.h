#ifndef RFID_H
#define RFID_H

#include <Arduino.h>        // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function
#include <MFRC522.h>

class RFID
{
public:
    RFID();

    void init();

    bool isCardPresent();

    uint32_t getCardID();

    void listen();

private:
    bool cardPresent;
    uint32_t cardID;
};

#endif // RFID_H