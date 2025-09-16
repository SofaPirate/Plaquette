/**
 * EnginesSlowFast
 *
 * Demonstrates the use of multiple Plaquette engines running at different pace using
 * metronomes. In this example, the primary engine runs two metronomes operating at a
 * slow (50 fps) and fast (1000 fps) paces. The slow engine manages the blinking of an
 * LED while the fast engine monitors interactions with a pushbutton.
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

// Metronomes (belong to primary engine).
Metronome slowMetro(0.05);  // 50 Hz
Metronome fastMetro(0.001); // 1000 Hz

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
  slowMetro.onBang(slowEngineStep);
  fastMetro.onBang(fastEngineStep);
}

void step() {}

void slowEngineStep() {
  slowEngine.step();

  // Adjust frequency and send to LED.
  wave.frequency(ledFrequency);
  wave >> led;
}

void fastEngineStep() {
  fastEngine.step();

  // On button press, increase square wave frequency.
  if (button.rose())
    ledFrequency++;
}
