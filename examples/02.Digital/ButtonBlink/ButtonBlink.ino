/**
 * ButtonBlink
 *
 * A button triggers the fast blinking of an LED.
 *
 * The circuit:
 * - LED attached from digital pin 13 to ground (*)
 * - pushbutton attached to digital pin 2 from ground
 * Note: on most Arduinos there is already an LED on the board
 * attached to pin 13.
 *
 * Created in 2016 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The LED.
DigitalOut led(LED_BUILTIN);

// The button.
DigitalIn button(2); // defaults to INTERNAL_PULLUP

// A square-wave (on/off) oscillator.
SquareOsc oscillator(0.1); // 100 ms period, 50% duty cycle (default)

void begin() {
  button.debounce(); // debounce button
}

void step() {
  // If button is pushed, send oscillator value to LED.
  if (button)
		oscillator >> led;
	// ... otherwise send 0 to LED (ie. equivalent of calling "led.off()").
	else
    0 >> led;

  // NOTE: The above code can be rewritten using a single line:
  // (button * oscillator) >> led;
}
