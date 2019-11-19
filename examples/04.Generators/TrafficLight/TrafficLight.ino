/**
 * TrafficLight
 *
 * This code controls a set of traffic lights that go: red, yellow, green, red,
 * yellow, green, and so on. It uses a sawtooth to iterate through these three
 * states.
 *
 * Created in 2019 by Sofian Audry
 *
 * This example code is in the public domain.
 */
 #include <Plaquette.h>

// The three LEDs used for the lights.
DigitalOut green(10);
DigitalOut yellow(11);
DigitalOut red(12);

// The triangle oscillator.
TriOsc osc(10.0); // 10 seconds period

void begin() {
  osc.width(1.0); // sawtooth wave
}

void step() {
  // Shut down all lights.
  0 >> red >> yellow >> green;
  // Switch on appropriate LED.
  if (osc < 0.4)
    green.on();
  else if (osc < 0.6)
    yellow.on();
  else
    red.on();
}
