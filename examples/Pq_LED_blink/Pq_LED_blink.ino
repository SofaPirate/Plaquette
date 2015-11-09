#include <Plaquette.h>

DigitalOut led; // defaults on pin 13
SquareOsc osc(2, 0.75); // square oscillator with period of 2 sec and 75% duty-cycle

void begin() {}

void run() {
  osc >> led;
}
