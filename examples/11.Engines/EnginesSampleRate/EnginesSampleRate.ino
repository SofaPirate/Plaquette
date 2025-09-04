/**
 * EnginesSampleRate
 *
 * Demonstrates the use of multiple Plaquette engines running at different pace using
 * target sample rates. In this example, the primary engine runs as fast as possible
 * and steps two engines operating at slow (50 fps) and fast (1000 fps) paces. The slow
 * engine manages the blinking of an LED while the fast engine monitors interactions
 * with a pushbutton.
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
Engine slowEngine;
Engine fastEngine;

// Button (operates on fast engine to monitor interactions).
DigitalIn button(2, INTERNAL_PULLUP, fastEngine);

// The square wave and LED (can operate more slowly to save on computation).
Wave wave(1.0, slowEngine);
DigitalOut led(LED_BUILTIN, slowEngine);

// Frequency of the square wave.
float ledFrequency = 1.0;

void begin() {
  // Initialize engines.
  slowEngine.begin();
  fastEngine.begin();

  // Debounce button.
  button.debounce();

  // Attach metronomes to engine step functions.
  slowEngine.sampleRate(50);
  fastEngine.sampleRate(1000);
}

void step() {
  slowEngineStep();
  fastEngineStep();
}

void slowEngineStep() {
  // IMPORTANT: Ensures that the rest of the function is executed at
  // correct sample rate by exiting early if the engine is not ready.
  if (!slowEngine.step())
    return;

  // Adjust frequency and send to LED.
  wave.frequency(ledFrequency);
  wave >> led;
}

void fastEngineStep() {
  // (See notice above).
  if (!fastEngine.step())
    return;

  // On button press, increase square wave frequency.
  if (button.rose())
    ledFrequency++;
}
