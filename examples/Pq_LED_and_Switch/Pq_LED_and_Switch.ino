#include <Plaquette.h>

DigitalOut led; // defaults on pin 13
DigitalIn button(2);

void begin() {}

void run() {
  button >> led;
}
