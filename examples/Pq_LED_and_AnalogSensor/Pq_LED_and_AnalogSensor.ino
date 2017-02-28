#include <Plaquette.h>

PwmOut led;  // defaults on pin 3
AnalogIn in; // defaults on pin A0

void init() {
  in.smooth(); // add some smoothing
}

void step() {
  // Sends input signal to LED intensity.
  in >> led;
}
