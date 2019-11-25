/**
 * Timer
 *
 * Turns on and off a light emitting diode (LED) at increasingly long
 * durations.
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
Timer timer(1.0); // starting duration: 1 second

// Keeps track of increasing duration of LED.
float ledDuration;

void begin() {
  // Initalize ledDuration to starting timer duration.
  ledDuration = timer.duration();

  // Start timer.
  timer.start();
}

void step() {
  // If timer has reached its duration, perform some actions.
  if (timer) {
    // Switch LED.
    led.toggle();

    // If LED is "on" keep it on for a certain time.
    if (led) {
      ledDuration += 0.5;
      timer.start(ledDuration);
    }
    // Otherwise keep it off for half a second.
    else
      timer.start(0.5);
  }
}
