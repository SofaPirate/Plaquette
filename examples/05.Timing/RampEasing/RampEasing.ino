/**
 * RampEasing
 *
 * Demonstrates the use of easing functions with ramps.
 * Sends values to the serial output: best visualized using the Arduino Serial Plotter.
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The ramp object.
Ramp ramp(2.0); // ramp duration: 2 seconds (initial value is zero)

// The serial output.
Plotter serialOut(115200);

void begin() {
  // Define ramp range.
  ramp.fromTo(-3, 3);

  // Apply easing function.
  // You can try with different easing functions. A full list is available at:
  // http://plaquette.org/Plaquette/easings.html
  ramp.easing(easeInSine);

  // Start ramp.
  ramp.start();
}

void step() {
  // When ramp is complete, re-launch it.
  if (ramp.isFinished())
  {
    // Restarts the ramp going from current value to a random value in [-10, +10] (keeping the same duration).
    ramp.start();
  }

  // Send ramp value to serial.
  ramp >> serialOut;
}
