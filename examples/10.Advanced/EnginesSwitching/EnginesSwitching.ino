/**
 * EnginesSwitching
 *
 * Demonstrates the use of multiple Plaquette engines allowing to switch between
 * them. In this example, the primary engine runs a sine wave while the secondary
 * engine runs a square wave.
 *
 * The circuit:
 * - Pushbutton: attached to digital pin 2 from ground
 * - Analog (PWM) LED:
 *   - LED cathode (short leg) attached to ground
 *   - LED anode (long leg) attached to 220-330 Ohm resistor
 *   - resistor attached to analog (PWM) output 9
 * - Digital LED: attached from digital pin 13 to ground
 * Note: on most Arduinos there is already an LED on the board
 * attached to pin 13.
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

Engine engine1;

Engine engine2;

// Digital LED.
DigitalOut led1(13, engine1);

// Sine oscillator.
SquareWave wave1(2.0, engine1); // period of 5 seconds

// Analog (PWM) LED.
AnalogOut led2(9, engine2);

// Sine oscillator.
SineWave wave2(2.0, engine2); // period of 5 seconds

// Push-button.
DigitalIn button(2, INTERNAL_PULLUP);

void begin() {
  // Initialize engines.
  engine1.begin();
  engine2.begin();

  // Debounce button.
  button.debounce();
}

void step() {
  // Switch between engines on button press.
  if (!button) {
    engine1.step();
    wave1 >> led1;
    println(wave1);
  }
  else {
    engine2.step();
    wave2 >> led2;
    println(wave2);
  }
}
