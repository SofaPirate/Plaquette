/**
 * PivotFieldSlider
 *
 * Demonstrates the use of PivotField, using a potentiometer to explore the different values
 * of the field and a button to explore modes.
 *
 * The circuit:
 *
 * - Pushbutton attached to digital pin 2 from ground
 * - Potentiometer #1 attached to analog input 0 center pin of the potentiometer
 *   to the analog pin: one side pin (either one) to ground the other side pin to +5V
 * - Potentiometer #2 attached to analog input 1 in the same way
 * - Analog LED:
 *   - LED cathode (short leg) attached to ground
 *   - LED anode (long leg) attached to 220-330 Ohm resistor
 *   - resistor attached to analog (PWM) output 9
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// Potentiometer #1 to control the pivot point.
AnalogIn pot(A0);

// Potentiometer #2 for exploring the field.
AnalogIn slider(A1);

// The PWM LED.
AnalogOut led(9);

// The pivot field.
PivotField field;

// The pushbutton.
DigitalIn button(2, INTERNAL_PULLUP);

void begin() {
  // Add ramp.
  field.rampWidth(0.2);

  // Easing.
  field.easing(easeInSine); // You can try with different easing functions.
}

void step() {

  // Change mode when button is pressed.
  if (button.rose()) {
    switch (field.mode()) {
      case PIVOT_FALLING: field.mode(PIVOT_RISING);  break;
      case PIVOT_RISING:  field.mode(PIVOT_BUMP);    break;
      case PIVOT_BUMP:    field.mode(PIVOT_NOTCH);   break;
      case PIVOT_NOTCH:   field.mode(PIVOT_FALLING); break;
    }
  }

  // Send potientiometer value to the pivot field.
  pot >> field;

  // Send field value at position of slider to LED.
  field.at(slider) >> led;
}

