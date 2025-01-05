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
Ramp zigZagRamp(2.0); // ramp duration: 2 seconds (initial value is zero)

// Serial output.
StreamOut serialOut(Serial);

void begin() {
  zigZagRamp.go(-10, 10); // first ramp from -10 to 10
}

void step() {
  // When ramp is complete, re-launch it.
  if (zigZagRamp.isFinished())
  {
    // Restarts the ramp going from current value to a random value in [-10, +10] (keeping the same duration).
    zigZagRamp.go( randomFloat(-10, 10) );
  }

  // Send ramp value to serial.
  zigZagRamp >> serialOut;
}
