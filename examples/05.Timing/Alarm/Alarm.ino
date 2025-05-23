/**
 * Alarm
 *
 * Turns on and off a light emitting diode (LED) at increasingly long
 * durations.
 *
 * The circuit:
 * - LED attached from pin 13 to ground.
 * Note: on most Arduinos, there is already an LED on the board
 * that's attached to pin 13, so no hardware is needed for this example.
 *
 * Created in 2022 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The LED.
DigitalOut led(LED_BUILTIN);

// The alarm.
Alarm ledAlarm(1.0); // starting duration: 1 second

void begin() {
  // Start timer.
  ledAlarm.start();
}

void step() {
  // If timer has reached its duration, perform some actions.
  if (ledAlarm) {
    // Switch LED.
    led.toggle();

    // If LED is "on" keep it on for a certain time.
    if (led) {
      // Increate alarm duration by half a second and restart.
      ledAlarm.duration( ledAlarm.duration() + 0.5 );
      ledAlarm.start();
    }
    // Otherwise keep it off for half a second.
    else
      ledAlarm.start(0.5);
  }
}
