/**
 * RampSpeed
 *
 * Compares two ramps: one that uses duration and the other that uses speed.
 *
 * The first ramp has a constant duration of one second.
 * The second ramp's speed is controlled by a potentiometer: at low speeds it does not have time
 * to complete before both ramps are relaunched; at high speeds it completes before the first one.
 *
 * The circuit:
 * - Potentiometer attached to analog input A0, center pin of the potentiometer
 *   to the analog pin, side pins to +5V and ground
 *
 * This example is best viewed using the Arduino Serial Plotter so as to compare the two ramps.
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The ramp objects.
Ramp rampWithDuration;
Ramp rampWithSpeed;

// Potentiometer used to control speed.
AnalogIn speedIn(A0);

// Serial plotter.
Plotter plotter(115200, "duration,speed");

// Launches both ramps.
void startRamps() {
  rampWithDuration.start();
  rampWithSpeed.start();
}

void begin() {
  // Initialize both ramps.
  rampWithDuration.duration(1.0f);
  rampWithSpeed.speed(1.0f);

  // Launch ramps.
  startRamps();
}

void step() {
  // Each time the duration ramp finishes, update speed and relaunch both ramps.
  if (rampWithDuration.finished()) {
    speedIn.mapTo(0.25f, 4.0f) >> rampWithSpeed.Speed(); // speed in [0.25, 4] change-per-second
    startRamps();
  }

  // Print ramps to serial (best viewed using the Arduino Serial Plotter).
  rampWithDuration >> plotter;
  rampWithSpeed >> plotter;
}
