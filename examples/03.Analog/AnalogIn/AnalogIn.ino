/**
 * AnalogIn
 *
 * Demonstrates analog input by reading an analog sensor on analog pin 0 and
 * controlling the intensity of a light emitting diode (LED) connected to digital
 * pin 13.
 *
 * The circuit:
 * - Potentiometer attached to analog input 0 center pin of the potentiometer
 *   to the analog pin: one side pin (either one) to ground the other side pin to +5V
 * - LED anode (long leg) attached to digital output 13
 * - LED cathode (short leg) attached to ground
 *
 * Created in 2016 by Sofian Audry
 *
 * This example code is in the public domain.
 *
 * Inspired from the following code:
 * http://www.arduino.cc/en/Tutorial/AnalogInput
 */
#include <Plaquette.h>

// The LED.
DigitalOut led(LED_BUILTIN);

// The analog input.
AnalogIn pot(A0);

// A square-wave (on/off) oscillator.
SquareOsc oscillator(1.0);

void begin() {}

void step() {
  // Set oscillation period according to value of analog input and send value to LED.
  // NOTE: Period is in interval [1.0, 5.0] seconds.
  oscillator.period(1.0 + 4.0*pot);
  oscillator >> led;

  // NOTE: These lines of code can be rewritten using a single line of code:
  // oscillator.period(1.0 + 4.0*pot) >> led;
}
