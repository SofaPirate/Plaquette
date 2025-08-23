/**
 * RawReadWrite
 *
 * Demonstrates the use of raw read and write functions. These functions allow
 * to access raw data from DigitalIn, DigitalOut, AnalogIn, and AnalogOut.
 * 
 * WARNING: These functions are provided for convenience and are not recommended 
 * for normal use.
 *
 * While a pushbutton is pressed, an LED increases to its maximum brightness
 * and then goes back to zero.
 * 
 * The circuit:
 * - LED cathode (short leg) attached to ground
 * - LED anode (long leg) attached to 220-330 Ohm resistor
 * - resistor attached to analog (PWM) output 9
 * - pushbutton attached to digital pin 2 from ground
 * Note: on most Arduinos there is already an LED on the board
 * attached to pin 13.
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The analog LED.
AnalogOut led(9);

// The pushbutton.
DigitalIn button(2, INTERNAL_PULLUP);

// The builtin LED.
DigitalOut indicatorLed(LED_BUILTIN);

// The LED value.
int ledValue = 0;

void begin() {
  // Initialize LED value.
  led.rawWrite(0);
}

void step() {
  // If button is pushed.
  if (button.rawRead() == LOW) { // button uses internal pullup
    // Turn on indicator LED.
    indicatorLed.rawWrite(HIGH);
    // Increase LED value.
    ledValue++;
    if (ledValue > 255)
      ledValue = 0;
    // Write value to LED.
    led.rawWrite(ledValue);
  }

  // Turn off indicator LED.
  else {
    indicatorLed.rawWrite(LOW);
  }
}