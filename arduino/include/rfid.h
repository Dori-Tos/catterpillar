#ifndef RFID_H
#define RFID_H

class RFID
{
public:
    RFID();

    void init();

    int getCardID(int i);

    void listen();

    int fromHexToInt(const String& hexString);

    void splitString(const String& str, char delimiter, String results[], size_t& count);

    String trimString(const String& str);

    void ConvertCardId();

private:
    bool cardPresent;
    String msg;
    int cardID[20]; // Initialize cardID array with size 20
};

#endif // RFID_H