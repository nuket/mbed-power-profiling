#include <events/mbed_events.h>
#include <mbed.h>

static EventQueue eventQueue(/* event count */ 8 * EVENTS_EVENT_SIZE);

// Important flags:
//
// 1. Highest power use `mbed compile -c`
// 2. Lower   power use `mbed compile -c --profile release -DENABLE_SLEEP`
// 3. Lowest  power use `mbed compile -c --profile release -DENABLE_SLEEP -DDISABLE_PRINTF`

// Turns out LEDs have minimal effect on Power Profiler when piggybacked 
// on an nRF52-DK, as the DK itself is simply sinking current.
//
// Thus, total project power would be higher if the DK is measured as an
// External DUT.
static DigitalOut alivenessLED(LED1, 0);

void blinkCallback(void)
{
    alivenessLED = !alivenessLED;

// Compile with `mbed compile -c -DDISABLE_PRINTF` to eliminate printf.

#ifndef DISABLE_PRINTF
    printf("Hello.\r\n");
#endif
}

int main()
{
#ifndef DISABLE_PRINTF
	// This will enable the UART hardware and never turn it off,
	// consuming a few hundred microamps forever.
    printf("Starting.\r\n");
#endif

    eventQueue.call_every(500, blinkCallback);

// Compile with `mbed compile -c --profile release -DENABLE_SLEEP` or 
// sleep() will not work. Release mode is required.

#ifdef ENABLE_SLEEP
    sleep();
#endif

    eventQueue.dispatch_forever();
    return 0;
}
