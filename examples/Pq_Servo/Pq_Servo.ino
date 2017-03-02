#include <Plaquette.h>
#include <PqServo.h>

// Controls the rate of change of the servo.
const float RATE = 0.01;

// The servo-motor output.
ServoOut out;

void begin() {
  // Centers the servo.
  out.center();
}

void step() {
  // Updates the value and send it back as output.
  (out + randomUniform(-RATE, RATE)) >> out;
}
