/**
 * PqPwmOut
 * Demonstrates analog (PWM) output by sending a ramping value into a LED output
 * connected to PWM pin 9.
 *
 * The circuit:
 * - LED anode (long leg) attached to digital output 13
 * - LED cathode (short leg) attached to ground
 *
 * Created in 2016 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The LED.
PwmOut led(9);

// Period during which to ramp up (seconds).
const float PERIOD = 5.0;

// Stores last time LED was updated.
float startTime = 0;

void begin() {}

void run() {
  // Check to see if it's time to blink the LED.
  float currentTime = seconds();

  // If currentTime has passed PERIOD, reset timer.
  if (currentTime - startTime >= PERIOD) {
    startTime = currentTime;
  }

  // Divide the result by PERIOD to project it in the range [0, 1].
  float ledValue = (currentTime - startTime) / PERIOD;

  // Send LED value to LED output.
  ledValue >> led;
}
