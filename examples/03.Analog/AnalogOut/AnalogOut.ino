/**
 * AnalogOut
 *
 * Demonstrates analog (PWM) output by sending a ramping value into a LED output
 * connected to PWM pin 9.
 *
 * The circuit:
 * - LED cathode (short leg) attached to ground
 * - LED anode (long leg) attached to 220-330 Ohm resistor
 * - resistor attached to analog (PWM) output 9
 *
 * Created in 2016 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The LED.
AnalogOut led(9);

// Sine oscillator.
SineOsc oscillator(5.0); // period of 5 seconds

void begin() {}

void step() {
	oscillator >> led;
}
