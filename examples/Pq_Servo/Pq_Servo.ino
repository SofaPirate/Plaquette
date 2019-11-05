#include <Plaquette.h>
#include <PqServo.h>

// Controls the rate of change of the servo.
const float RATE = 0.01;

// The servo-motor output on pin 9.
ServoOut out(9);

void begin() {
  // Centers the servo.
  out.center();
}

void step() {
  // Updates the value and send it back as output.
  (out + randomFloat(-RATE, RATE)) >> out;
}
