#include <arduino.h>
#include "accelerometer.h"
#include "ttn_handler.h"
#include "payload_builder.h"

unsigned long acc_timer = millis();
const int taskDelay = 100;

Accelerometer accelerometer(false);

void buildPayload()
{
    PayloadBuilder::buildAccelerationPayload(TTNHandler::payload, accelerometer);
}

void setup()
{
    delay(1000);
    while (!Serial)
        ;
    Serial.begin(115200);

    accelerometer.init();

    TTNHandler::init();
}

void loop()
{
    // we call the LMIC's runloop processor. This will cause things to happen based on events and time. One
    // of the things that will happen is callbacks for transmission complete or received messages. We also
    // use this loop to queue periodic data transmissions.  You can put other things here in the `loop()` routine,
    // but beware that LoRaWAN timing is pretty tight, so if you do more than a few milliseconds of work, you
    // will want to call `os_runloop_once()` every so often, to keep the radio running.
    os_runloop_once();

    accelerometer.read();

    if (millis() - acc_timer > taskDelay)
    {
        buildPayload();
    }
}
