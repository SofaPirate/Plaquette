/**
 * ScalerCalibration
 *
 * Rescales an analog input using a scaler unit, with a calibration period at the beginning.
 *
 * The program will calibrate itself for the first 30 seconds, after which it will
 * stay stable. Upon program launch, try different light conditions (eg. switch
 * interior lights on/off, cover photocell, etc.) during the calibration period.
 *
 * The circuit:
 * - A photoresistor connected to analog pin and to +5V
 * - A resistor (around 10k is a good value, higher values gives higher readings) from pin 0 to GND
 * - LED cathode (short leg) attached to ground
 * - LED anode (long leg) attached to 220-330 Ohm resistor
 * - resistor attached to analog (PWM) output 9
 *
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 *
 * Inspired from the following code:
 * http://www.arduino.cc/en/Tutorial/AnalogInput
 */
#include <Plaquette.h>

// Calibration time in seconds.
const float CALIBRATION_TIME = 30.0f;

// The LED.
AnalogOut led(9); // PWM pin 9

// The analog input.
AnalogIn in(A0);

// This unit will rescale signal to full range [0, 1].
RobustScaler scaler;

void begin() {}

void step() {
  // After 30 seconds, stop calibration. The scaler will still rescale
  // values but will stop updating its .
  if (scaler.isCalibrating() && seconds() >= CALIBRATION_TIME)
    scaler.pauseCalibrating(); // stop calibration

	// Analog input is rescaled then sent as LED value.
	in >> scaler >> led;
}
