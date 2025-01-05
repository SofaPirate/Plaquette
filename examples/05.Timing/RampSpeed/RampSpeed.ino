/**
 * RampSpeed
 *
 * Ramps to value send by user through the serial. Compares two ramps: one that uses duration and the
 * other that uses speed.
 * 
 * This example is best viewed using the Arduino Serial Plotter so as to compare the two ramps.
 * The first ramp has a constant duration of one second.
 * The second ramp's speed can be changed by the user through the serial.
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The ramp objects.
Ramp rampWithDuration;
Ramp rampWithSpeed;

// Serial input.
StreamIn serialIn(Serial);

void begin() {
  // Initialize both ramps.
  rampWithDuration.duration(1.0f);
  rampWithSpeed.speed(1.0f);

  // Launch ramps.
  startRamps();
}

void step() {
  // When serial receives new value, change speed and relaunch.
  if (serialIn.changed()) {
    rampWithSpeed.speed(serialIn);
    startRamps();
  }

  // Print ramps to serial (best viewed using the Arduino Serial Plotter).
  printRamps();
}

void startRamps() {
  rampWithDuration.start();
  rampWithSpeed.start();  
}

void printRamps() {
  Serial.print(rampWithDuration);
  Serial.print(" ");
  Serial.println(rampWithSpeed);
}