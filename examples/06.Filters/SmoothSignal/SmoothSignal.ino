/**
 * SmoothSignal
 *
 * Smooths an input signal coming from the serial.
 *
 * Created in 2022 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// Serial input.
StreamIn serial;

// Smoothing unit.
Smoother smoother(1.5f); // smooths over window of 1.5 seconds

// Rescaling unit.
MinMaxScaler scaler;

// Output LED.
AnalogOut led(9); // PWM pin 9

void begin() {}

void step() {
	// Smooth serial input, rescale in [0, 1], then send to LED.
	serial >> smoother >> scaler >> led;
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
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 115200);
}

void draw() {
  // Draw moving circle along x axis.
  background(0);
  fill(255);
  noStroke();
  ellipse(mouseX, height/2, 50, 50);
  // Send raw mouseX value to serial.
  myPort.write(mouseX + "\n");
}

 */
