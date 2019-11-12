/**
 * ZigZag
 *
 * Sequentially ramps through different values.
 * Sends values to the serial output: best visualized using the Arduino Serial Plotter.
 *
 * Created in 2019 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The ramp object.
Ramp zigZagRamp(0.0); // the ramp is initalized at 0

// Serial output.
StreamOut serialOut(Serial);

void begin() {}

void step() {
  // When ramp is complete, re-launch it.
  if (zigZagRamp.isComplete())
  {
    // Restarts the ramp going from current value to a random value in [-10, +10] in 2 seconds
    zigZagRamp.start(randomFloat(-10, 10), 2.0);
  }

  // Send ramp value to serial.
  zigZagRamp >> serialOut;
}
