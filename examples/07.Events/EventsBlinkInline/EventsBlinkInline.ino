/**
 * EventsBlinkInline
 *
 * Uses events with an inline function declaration to turn on and off a 
 * light emitting diode (LED) connected to a digital pin.
 *
 * The circuit:
 * - LED attached from pin 13 to ground.
 * Note: on most Arduinos, there is already an LED on the board
 * that's attached to pin 13, so no hardware is needed for this example.
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The LED.
DigitalOut led(LED_BUILTIN);

// A metronome.
Metronome metronome(0.5); // 1 second period

void begin() {
  metronome.onBang(
    // Inline callback function declaration. It is a convenient way to 
    // create a short callback function.
    []() { led.toggle(); }
  );
}

void step() {
}
