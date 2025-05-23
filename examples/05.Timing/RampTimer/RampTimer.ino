/**
 * RampTimer
 *
 * Boosts up LED value from 0% to 100% over a random duration
 * each time.
 *
 * The circuit:
 * - LED cathode (short leg) attached to ground
 * - LED anode (long leg) attached to 220-330 Ohm resistor
 * - resistor attached to analog (PWM) output 9
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The LED.
AnalogOut led(9);

// The timer.
Ramp timer;

// This function restarts the timer with a random duration.
void restartTimer() {
  timer.duration(randomFloat(1.0, 10.0)); // duration between 1 and 10 seconds
  timer.start();
}

void begin() {
  restartTimer();
}

void step() {
  // Send timer progress from 0..1 to LED value.
  timer >> led;

  // If timer has run its course, restart it.
  if (timer.isFinished())
    restartTimer();
}