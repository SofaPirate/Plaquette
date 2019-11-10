/**
 * SerialOut
 *
 * Demonstrates the use of stream output.
 *
 * The circuit:
 * - Potentiometer attached to analog input 0 center pin of the potentiometer
 *   to the analog pin: one side pin (either one) to ground the other side pin to +5V
 *
 * To use, open the serial monitor (CTRL+SHIFT+M) and select "9600 baud".
 *
 * Created in 2017 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The analog input.
AnalogIn in(A0);

// The serial input.
// TIP: try replacing the line by:
//     OscilloscopeOut serialOut;
StreamOut serialOut;

void begin() {}

void step() {
  // Send analog input value to serial.
  in >> serialOut;
}
