#include <Plaquette.h>

PwmOut led(3, SINK); // output to pin 3, LED set as a sink (inverted)
AnalogIn in;         // defaults on pin A0

SerialOut serial;

// Notice that baudrate is automatically set at 115200.
void init() {
  in.smooth(); // add some smoothing
}

void step() {
  // Sends input signal to LED intensity, then to serial.
  in >> led >> serial;
}
