#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>
#include <pq_random.h>

using namespace pq;
using namespace aunit;

#define N_UNITS 4
Wave oscillators[] = {Wave(1), Wave(2), Wave(3), Wave(4)};
DigitalOut leds[] = { 0, 1, 2, 3 };

test(nUnits) {
  assertEqual(Plaquette.nUnits(), (size_t)2*N_UNITS);
}

test(properties) {
  for (int i=0; i<N_UNITS; i++) {
    assertEqual(oscillators[i].period(), float(i+1));
    assertEqual(leds[i].pin(), i);
  }
}

testing(basic) {
  static unsigned long startTime = millis();

  Plaquette.step();

  for (int i=0; i<N_UNITS; i++) {
    Wave& osc = oscillators[i];
    DigitalOut& led = leds[i];
    osc >> led;
    assertEqual(osc.get(), led.get());
  }
  if (millis() - startTime > 5000) {
    pass();
  }
}

void setup() {
  Plaquette.begin();
  for (int i=0; i<N_UNITS; i++) {
    oscillators[i].phase(mapTo01(i, 0, N_UNITS));
  }
}

void loop() {
  aunit::TestRunner::run();
}
