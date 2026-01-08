/**
 * SerialPlotter
 *
 * Demonstrates the use of the serial plotter which simplifies the output of
 * signals for easy visualization.
 *
 * The circuit:
 * - Potentiometer attached to analog input 0 center pin of the potentiometer
 *   to the analog pin: one side pin (either one) to ground the other side pin to +5V
 *
 * In Arduino: To use, open the Serial Plotter and select "115200 baud".
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The analog input.
AnalogIn in(A0);

// A sine wave modulated with the input.
Wave wave(1.0);

// The serial monitor. Baudrate: 115200.
Plotter plotter(115200);

// Alternatively you can declare a sequence of labels to use:
// Plotter plotter(115200, "input,wave");

void step() {
  // Change the wave's skew.
  in >> wave.Skew();

  // Plot values.
  in >> plotter;
  wave >> plotter;
}
