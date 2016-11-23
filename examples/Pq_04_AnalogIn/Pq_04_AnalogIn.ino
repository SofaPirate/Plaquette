/**
 * PqAnalogIn
 * Demonstrates analog input by reading an analog sensor on analog pin 0 and
 * controlling the intensity of a light emitting diode (LED) connected to digital
 * pin 13.
 *
 * The circuit:
 * - Potentiometer attached to analog input 0 center pin of the potentiometer
*    to the analog pin one side pin (either one) to ground the other side pin to +5V
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
DigitalOut led; // defaults on pin 13

// The analog input.
AnalogIn in; // default on pin A0

// A square-wave (on/off) oscillator.
SquareOsc oscillator; // default to 1 second period and 50% duty-cycle

void begin() {}

void run() {
	// Set duty cycle of oscillator to value of analog input.
	oscillator.setDutyCycle(in);
  // Send value to LED.
  oscillator >> led;
}
