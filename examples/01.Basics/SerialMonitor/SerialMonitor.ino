/**
 * SerialMonitor
 *
 * Demonstrates the use of the serial monitor.
 *
 * The circuit:
 * - Potentiometer attached to analog input 0 center pin of the potentiometer
 *   to the analog pin: one side pin (either one) to ground the other side pin to +5V
 *
 * In Arduino: To use, open the Serial Monitor (CTRL+SHIFT+M) and select "115200 baud".
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The analog input.
AnalogIn in(A0);

// The serial monitor. Baudrate: 115200.
// Declaring it will automatically activate print()/println() functions on serial.
Monitor monitor(115200);

void step() {
  // Send analog input value to serial.
  print("Value: ");
  in >> monitor; // print value
  println();     // newline
}
