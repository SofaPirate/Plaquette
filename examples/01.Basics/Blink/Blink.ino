/**
 * BlinkOsc
 *
 * Turns on and off a light emitting diode (LED) connected to a digital
 * pin using a SquareOsc.
 *
 * The circuit:
 * - LED attached from pin 13 to ground.
 * Note: on most Arduinos, there is already an LED on the board
 * that's attached to pin 13, so no hardware is needed for this example.
 *
 * Created in 2016 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The LED.
DigitalOut led(LED_BUILTIN);

// A square-wave (on/off) oscillator.
SquareOsc oscillator(2.0); // 2 seconds period

void begin() {
  oscillator.dutyCycle(0.1);
}

void step() {
  // Send oscillator value to LED.
  oscillator >> led;
}
