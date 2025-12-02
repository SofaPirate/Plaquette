/**
 * VisualizeMinMaxScaler
 *
 * Visualizes how a MinMaxScaler unit rescales a noisy signal.
 *
 * You can visualize the results using the Arduino's Serial Plotter.
 *
 * The circuit:
 * - A photoresistor connected to analog pin and to +5V
 * - A resistor (around 10k is a good value, higher values gives higher readings) from pin 0 to GND
 * - LED cathode (short leg) attached to ground
 * - LED anode (long leg) attached to 220-330 Ohm resistor
 * - resistor attached to analog (PWM) output 9
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 *
 * Inspired from the following code:
 * http://www.arduino.cc/en/Tutorial/AnalogInput
 */
#include <Plaquette.h>

// Controls the "memory" of the scaler: the scaler will slowly forget over that many seconds.
const float TIME_WINDOW = 60.0; // try changing this to see the effect

// The input sensor.
AnalogIn sensor(A0);

// The robust scaler with a
MinMaxScaler scaler(TIME_WINDOW);

void begin() {}

void step() {

  // Send sensor value to scaler.
  sensor >> scaler;

  // Prints values for comparison using the Serial plotter.
  print("sensor:"); print(sensor); print(" ");
  print("scaler:"); print(scaler); print(" ");
  print("max:");    print(scaler.maxValue()); print(" ");
  print("min:");    print(scaler.minValue()); println();
}
