/**
 * Metronome
 *
 * Uses the Metro class and the Timer class to simulate a metronome.
 *
 * The circuit:
 * - Potentiometer attached to analog input 0 center pin of the potentiometer
 *   to the analog pin: one side pin (either one) to ground the other side pin to +5V
 * - LED anode (long leg) attached to digital output 13
 * - LED cathode (short leg) attached to ground
 *
 * Created in 2018 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The LED.
DigitalOut led(LED_BUILTIN);

// The analog input.
AnalogIn pot(A0);

// The metronome object.
Metro metronome(1.0);

// The timer object (to make sure the LED light stays visible for a short time).
Timer ledOnTimer(0.05);

void begin() {
  // Start with LED off.
  led.off();
}

void step() {
  // Adjust metronome period with potentiometer from 100ms to 2 seconds.
  metronome.period(pot.mapTo(0.1, 2.0));

  // When the metronome "hits": switch LED on and start timer.
  if (metronome) {
    led.on();
    ledOnTimer.start();
  }

  // Switch off LED when timer has completed.
  if (ledOnTimer)
    led.off();
}
