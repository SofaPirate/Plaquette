/**
 * Pq_SerialOut
 * Demonstrates the use of stream output.
 *
 * The circuit:
 * - A photoresistor connected to analog pin and to +5V
 * - A resistor (around 10k is a good value, higher values gives higher readings) from pin 0 to GND
 *
 * To use, open the serial monitor (CTRL+SHIFT+M) and select "115200 baud".
 *
 * Created in 2017 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The analog input.
AnalogIn in; // default on pin A0

// The serial input.
StreamOut serialOut;

void begin() {}

void step() {
  // Send analog input value to serial.
  in >> serialOut;
}
