#ifndef RFID_H
#define RFID_H

#include <Arduino.h>        // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function


class RFID
{
public:
    RFID();

    void init();

    bool isCardPresent();

    String getCardID();

    void listen();
    
    
private:
    bool cardPresent;
    String cardID;
};

#endif // RFID_H