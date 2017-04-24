#include <events/mbed_events.h>
#include <mbed.h>

static EventQueue eventQueue(/* event count */ 8 * EVENTS_EVENT_SIZE);

#define  LOWER_POWER 0
#define LOWEST_POWER 0  // Must compile this with `--profile release` or sleep() will not work.

// Turns out LEDs have minimal effect on Power Profiler when piggybacked 
// on an nRF52-DK, as the DK itself is simply sinking current.
//
// Thus, total project power would be higher if the DK is measured as an
// External DUT.
static DigitalOut alivenessLED(LED1, 0);

void blinkCallback(void)
{
    alivenessLED = !alivenessLED;
   
#if LOWER_POWER == 0
    printf("Hello.\r\n");
#endif
}

int main()
{
    eventQueue.call_every(500, blinkCallback);

#if LOWEST_POWER
    sleep();
#endif

    eventQueue.dispatch_forever();
    return 0;
}