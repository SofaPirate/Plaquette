#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>

using namespace pq;

#define N_GENERATORS 3
Node* generators[N_GENERATORS] = {
  new SquareOsc(0.5),
  new TriOsc(0.5),
  new SineOsc(0.5)
};

testing(valuesIn01) {
  static unsigned long startTime = millis();
  Plaquette.step();

  for (int i=0; i<N_GENERATORS; i++) {
    Node* unit = generators[i];
    float value = unit->get();
    assertMoreOrEqual(value, 0.0f);
    assertLessOrEqual(value, 1.0f);
  }
  if (millis() - startTime > 1000) pass();
}

test(nUnits) {
  assertEqual((int)Plaquette.nUnits(), N_GENERATORS);
}

void setup() {
  Plaquette.begin();
  Plaquette.sampleRate(1000);
}

void loop() {
  aunit::TestRunner::run();
}
