/**
 * SmoothSignal
 *
 * Smooths an analog input signal, then rescales it to control an LED.
 *
 * The circuit:
 * - Potentiometer attached to analog input A0, center pin of the potentiometer
 *   to the analog pin, side pins to +5V and ground
 * - LED cathode (short leg) attached to ground
 * - LED anode (long leg) attached to 220-330 Ohm resistor
 * - resistor attached to analog (PWM) output 9
 *
 * This example is best viewed using the Arduino Serial Plotter so as to compare the raw and
 * smoothed signals.
 *
 * Created in 2022 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// Analog input.
AnalogIn in(A0);

// Smoothing unit.
Smoother smoother(1.5f); // smooths over window of 1.5 seconds

// Rescaling unit.
MinMaxScaler scaler;

// Output LED.
AnalogOut led(9); // PWM pin 9

// Serial plotter.
Plotter plotter(115200, "raw,smoothed");

void begin() {}

void step() {
  // Smooth input, rescale in [0, 1], then send to LED.
  in >> smoother >> scaler >> led;

  // Print raw and smoothed values (best viewed using the Arduino Serial Plotter).
  in >> plotter;
  smoother >> plotter;
}
