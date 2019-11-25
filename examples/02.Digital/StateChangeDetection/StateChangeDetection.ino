/**
 * StateChangeDetection
 *
 * Often, you don't need to know the state of a digital input all the time, but
 * you just need to know when the input changes from one state to another.
 * For example, you want to know when a button goes from OFF to ON. This is called
 * state change detection, or edge detection.
 *
 * The circuit:
 * - LED attached from digital pin 13 to ground (*)
 * - pushbutton attached to digital pin 2 from ground
 * Note: on most Arduinos there is already an LED on the board
 * attached to pin 13.
 *
 * Created in 2019 by Sofian Audry
 * Based on example created 27 Sep 2005, modified 30 Aug 2011, by Tom Igoe
 *
 * This example code is in the public domain.
 *
 * Inspired from the following code:
 * http://www.arduino.cc/en/Tutorial/ButtonStateChange
 */
#include <Plaquette.h>

// The LED.
DigitalOut led(LED_BUILTIN);

// The button.
DigitalIn button(2); // defaults to INTERNAL_PULLUP

// Counter for the number of button presses.
int buttonPushCounter = 0;

void begin() {
  button.debounce(); // debounce button
}

void step() {
  if (button.changed())
    println("changed");

  // Check if button went from false to true (pressed)
  if (button.rose()) {
    buttonPushCounter++;
    println("on");
    print("number of button pushes: ");
    println(buttonPushCounter);
  }
  // Check if button went from true to false (released)
  else if (button.fell()) {
    println("off");
  }

  // Turns on the LED every four button pushes by checking the modulo of the
  // button push counter. The modulo function gives you the remainder of the
  // division of two numbers.
  // NOTE: These lines of code can be rewritten using a single line of code:
  // (buttonPushCounter % 4 == 0) >> led;
  if (buttonPushCounter % 4 == 0) {
    led.on();
  } else {
    led.off();
  }
}
