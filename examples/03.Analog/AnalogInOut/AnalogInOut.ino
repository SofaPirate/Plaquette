/**
 * AnalogInOut
 *
 * Directly sends smoothed analog input signal to pwm output.
 *
 * The circuit:
 * - Potentiometer attached to analog input 0 center pin of the potentiometer
 *   to the analog pin: one side pin (either one) to ground the other side pin to +5V
 * - LED cathode (short leg) attached to ground
 * - LED anode (long leg) attached to 220-330 Ohm resistor
 * - resistor attached to analog (PWM) output 9
 *
 * Created in 2016 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The analog input.
AnalogIn pot(A0);

// The LED.
AnalogOut led(9);

void begin() {}

void step() {
  // Just send input to output.
  pot >> led;
}
