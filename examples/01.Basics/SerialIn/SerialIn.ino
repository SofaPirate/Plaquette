/**
 * SerialIn
 *
 * Demonstrates the use of stream input.
 *
 * The circuit:
 * - LED cathode (short leg) attached to ground
 * - LED anode (long leg) attached to 220-330 Ohm resistor
 * - resistor attached to analog (PWM) output 9
 *
 * To use, open the serial monitor (CTRL+SHIFT+M) and select "9600 baud".
 * Then type a value between 0 and 1 and press ENTER to set the intensity of the LED.
 *
 * Created in 2017 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The LED.
AnalogOut led(9);

// The serial input.
StreamIn serialIn;

void begin() {}

void step() {
	// Send value to LED.
 serialIn >> led;
}
