/**
 * Demonstrates the use of events with a button.
 *
 * The circuit:
 * - LED attached from digital pin 13 to ground (*)
 * - pushbutton attached to digital pin 2 from ground (optional)
 * Note: on most Arduinos there is already an LED on the board
 * attached to pin 13.
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The LED.
DigitalOut led(LED_BUILTIN);

// The button.
DigitalIn button(2, INTERNAL_PULLUP);

// Serial monitor.
// Seclaring this will automatically activate print()/println()
Monitor monitor(115200);

void begin() {
  button.debounce(); // debounce button

  // Register events.
  button.onRise(buttonPressed);
  button.onFall(buttonReleased);
  button.onChange(buttonChanged);

  // Additional event registered on button pressed.
  button.onRise(toggleLed);
}

void step() {
}

void buttonPressed() {
  println("Button pressed");
}

void buttonReleased() {
  println("Button released");
}

void buttonChanged() {
  println("Button changed");
}

void toggleLed() {
  led.toggle();
}
