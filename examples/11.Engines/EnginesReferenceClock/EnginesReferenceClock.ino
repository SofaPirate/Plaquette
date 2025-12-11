/**
 * EnginesReferenceClock
 *
 * Demonstrates the use of a custom reference clock on Plaquette engines. A pushbutton is used to
 * mark a beat, which is reinterpreted to set the clock of a secondary engine.
 *
 * The circuit:
 * - LED attached from digital pin 13 to ground (*)
 * - pushbutton attached to digital pin 2 from ground
 * Note: on most Arduinos there is already an LED on the board
 * attached to pin 13.
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The engines.
Engine customClockEngine;

// Button (operates on fast engine to monitor interactions).
DigitalIn button(2, INTERNAL_PULLUP);

// Counts time between button presses.
Chronometer intervalChrono;

// Smooths interval over a 10 seconds time window.
Smoother intervalSmoother(10.0);

// The square wave and LED (can operate more slowly to save on computation).
Wave wave(1.0, customClockEngine);
DigitalOut led(LED_BUILTIN, customClockEngine);

// Custom reference clock.
unsigned long customMicros = 0;   // current reference time in microseconds

// Returns current reference time in microseconds.
unsigned long customMicroSeconds() { return customMicros; }

// Previous step in microseconds (in primary engine basic "true" time).
unsigned long prevStepMicros = 0;

void settings() {
  customClockEngine.referenceClock(customMicroSeconds);
}

void begin() {
  // Initialize engines.
  customClockEngine.begin();

  // Debounce button.
  button.debounce();
  intervalChrono.start();

  // Initialize smoother with a 1:1 ratio.
  intervalSmoother.reset(1.0);
}

void step() {
  // Button press: adjust interval.
  if (button.rose()) {
    // Smooth interval.
    if (intervalChrono <= max(5*intervalSmoother, 1)) // Ignore too long intervals (means the user has waited and is now starting a new beat)
      intervalChrono >> intervalSmoother;

    // Restart chrono.
    intervalChrono.start();
  }

  // Compute time interval between steps (in "true" time).
  float trueMicrosInterval = microSeconds() - prevStepMicros;
  prevStepMicros = microSeconds();

  // Adjust microseconds update to true_interval_micros / interval_seconds
  customMicros += trueMicrosInterval / intervalSmoother;

  // Step custom engine.
  customClockEngineStep();
}

void customClockEngineStep() {
  // IMPORTANT: Ensures that the rest of the function is executed at
  // correct sample rate by exiting early if the engine is not ready.
  if (!customClockEngine.step())
    return;

  // Wave will operate at 1 Hz according to the custom reference clock.
  wave >> led;
}
