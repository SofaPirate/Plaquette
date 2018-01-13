/**
 * PqAnalogInToOutScaled
 * Rescales an analog input using the min-max scaler.
 *
 * The circuit:
 * - A photoresistor connected to analog pin and to +5V
 * - A resistor (around 10k is a good value, higher values gives higher readings) from pin 0 to GND
 * - LED anode (long leg) attached to digital output 13
 * - LED cathode (short leg) attached to ground
 *
 * Created in 2016 by Sofian Audry
 *
 * This example code is in the public domain.
 *
 * Inspired from the following code:
 * http://www.arduino.cc/en/Tutorial/AnalogInput
 */
#include <Plaquette.h>

// The LED.
AnalogOut led(9); // PWM pin 9

// The analog input.
AnalogIn in(A0);

// This object will rescale according to min and max values.
MinMaxScaler scaler;

void begin() {}

void step() {
	// Analog input is rescaled then sent as LED value.
	in >> scaler >> led;
}
