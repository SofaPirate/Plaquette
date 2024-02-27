#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>
#include <pq_random.h>

using namespace pq;
using namespace aunit;

#define N_UNITS 4
SquareOsc oscillators[] = {SquareOsc(1), SquareOsc(2), SquareOsc(3), SquareOsc(4)};
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
  static uint16_t nPeaks = 0;

  Plaquette.step();

  for (int i=0; i<N_UNITS; i++) {
    SquareOsc& osc = oscillators[i];
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
  Plaquette.sampleRate(10000);
}

void loop() {
  aunit::TestRunner::run();
}
