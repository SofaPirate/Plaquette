/**
 * PivotFieldMeter
 *
 * Demonstrates the use of PivotField and arrays to create a VU-meter with 8 LEDs.
 *
 * The circuit:
 *
 * - Array of LEDs:
 *   - Eight LEDs.
 *   - The anode of the LEDs are connected in series with a 220-ohm resistor to pins 2 to 9.
 *   - Their cathodes connect to ground.
 * - Sensor:
 *   - Connect the sensor (eg. photocell) and a fixed resistor (eg. 1k) in series between Vcc
 *     and GND, with the junction to an analog pin.
 *   - Choose the fixed resistor to GND so the analog value rises as light decreases; or the
 *     other way around if you want the opposite behavior.
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The number of LEDs.
const int N_LEDS = 8;

// An analog sensor such as a potentiometer or photocell.
AnalogIn sensor(A0);

// The array of LEDs.
DigitalOut leds[] = { 2, 3, 4, 5, 6, 7, 8, 9 }; // shorthand for DigitalOut leds[] = { DigitalOut(2), DigitalOut(3), DigitalOut(4), ... };

// The pivot field.
PivotField meter;

void begin() {
  // You can experiment with changing the field's parameters to see how they affect the output:

  // Center:
  // meter.center(0); // default: falling curve from the left
  // meter.center(1); // default: falling curve from the right
  // meter.center(0.5); // default: falling curve from the center

  // Mode:
  // meter.mode(PIVOT_FALLING); // default: falling curve
  // meter.mode(PIVOT_RISING); // rising curve from the right
  // meter.mode(PIVOT_BUMP); // bump at pivot point
  // meter.mode(PIVOT_NOTCH); // notch at pivot point
}

void step() {
  // Update the pivot field.
  sensor >> meter;

  // Update the LEDs.
  meter.populate(leds, N_LEDS);
}
