/**
 * BlinkTimer
 *
 * Turns on and off a light emitting diode (LED) using a Timer unit.
 *
 * The circuit:
 * - LED attached from pin 13 to ground.
 * Note: on most Arduinos, there is already an LED on the board
 * that's attached to pin 13, so no hardware is needed for this example.
 *
 * Created in 2018 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The LED.
DigitalOut led(LED_BUILTIN);

// The timer.
Timer timer(1.0); // period: 1 second

void begin() {
  // Start timer.
  timer.start();
}

void step() {
  // Every time the metronome "fires": change LED state.
  if (timer) {
    // Flip LED state.
    led.toggle();

    // Restart timer.
    timer.start();
  }
}
