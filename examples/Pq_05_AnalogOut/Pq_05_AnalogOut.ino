/**
 * Pq_AnalogOut
 * Demonstrates analog (PWM) output by sending a ramping value into a LED output
 * connected to PWM pin 9.
 *
 * The circuit:
 * - LED anode (long leg) attached to analog (PWM) output 9
 * - LED cathode (short leg) attached to ground
 *
 * Created in 2016 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The LED.
AnalogOut led(9);

// The triangle/saw oscillator.
/* NOTES:
   - try changing the second parameter to 0 and 1 and see how it affects the LED
   - try replacing the line with the following:
       SineOsc oscillator(5.0);
*/
TriOsc oscillator(5.0); // period of 5 seconds, width of 50% (default)

void begin() {}

void step() {
	oscillator >> led;
}
