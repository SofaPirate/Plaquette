/**
 * Smooth
 *
 * Demonstrates analog input smoothing.
 *
 * The circuit:
 * - Potentiometer attached to analog input 0 center pin of the potentiometer
 *   to the analog pin: one side pin (either one) to ground the other side pin to +5V
 *
 * Created in 2016 by Sofian Audry
 *
 * This example code is in the public domain.
 *
 * Inspired from the following code:
 * http://www.arduino.cc/en/Tutorial/AnalogInput
 */
#include <Plaquette.h>

// The analog input.
AnalogIn in(A0);

// The serial input.
StreamOut serialOut;

void begin() {
  // Smooth over a window of 100ms (ie. one tenth of a second).
  // NOTE: Try changing the smoothing value to see how it affects the outputs.
  in.smooth(0.1);
}

void step() {
  // Send value to serial.
  in >> serialOut;
}
