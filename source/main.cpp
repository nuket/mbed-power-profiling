/*
    mbed Power Consumption Tests
    Copyright (c) 2017 Max Vilimpoc
    
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

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
