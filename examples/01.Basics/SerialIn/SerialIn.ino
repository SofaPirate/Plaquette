/**
 * SerialIn
 *
 * Demonstrates the use of stream input.
 *
 * The circuit:
 * - LED cathode (short leg) attached to ground
 * - LED anode (long leg) attached to 220-330 Ohm resistor
 * - resistor attached to analog (PWM) output 9
 *
 * To use, open the serial monitor (CTRL+SHIFT+M) and select "9600 baud".
 * Then type a value between 0 and 1 and press ENTER to set the intensity of the LED.
 *
 * Created in 2017 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The LED.
AnalogOut led(9);

// The serial input.
StreamIn serialIn;

void begin() {}

void step() {
	// Send value to LED.
 serialIn >> led;
}

/*
// Processing code (processing.org). Use it to send values from
// the computer to the Plaquette program by serial.

import processing.serial.*;

Serial myPort;

void setup()
{
  size(500, 200);

  // Open whatever port is the one you're using.
  // To print a list run: println(Serial.list());
  myPort = new Serial(this, "/dev/ttyUSB0", 115200);
  //myPort = new Serial(this, "/dev/ttyACM0", 115200);
  //myPort = new Serial(this, "COM1", 115200);

  // Slow down frame rate to avoid overflowing the serial.
  frameRate(20);
}

void draw() {
  // Draw moving circle along x axis.
  background(0);
  fill(255);
  noStroke();
  ellipse(mouseX, height/2, 50, 50);
  // Send value in [0, 1] to serial depending on mouse position.
  myPort.write(map(mouseX, 0, width, 0, 1) + "\n");
}

 */
