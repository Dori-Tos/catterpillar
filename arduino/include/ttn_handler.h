#ifndef TTN_HANDLER_H
#define TTN_HANDLER_H

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <stdint.h>

class TTNHandler
{
public:
    static void init();

    static void do_send(osjob_t *j);

    static void onEvent(ev_t ev);

    static uint8_t payload[16];

private:
    static const unsigned TX_INTERVAL;
    static osjob_t sendjob;
};

#endif // TTN_HANDLER_H