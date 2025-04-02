/**
 * Operations
 *
 * Demonstrates the different kinds of mathematical and logical operations available with
 * Plaquette objects.
 *
 * Created in 2017 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// Analog input.
AnalogIn in(A0);

// Digital input.
DigitalIn button(2, INTERNAL_PULLUP);

// Analog output.
AnalogOut led(9);

// Main sine oscillator (period of 1 sec).
SineWave oscillator(1.0);

// Sine oscillator with period of 10 sec used to modulate oscillator.
SineWave modulator(10.0);

// Default square oscillator.
SquareWave baseOscillator(2.0);

void begin() {}

void step() {
  // if button is pressed OR if input signal is big enough then use first oscillator
  if (button || in > 0.2)
    baseOscillator >> led;

  // else modulate by changing the period between 0.5 and 2.5 seconds
  else {
    oscillator.period(modulator.mapTo(0.5, 2.0));
    oscillator >> led;
  }
}
