#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>

using namespace pq;

#define N_UNITS 3
Node* units[N_UNITS] = {
  new Normalizer(0, 1),
  new Normalizer(0, 1, 0.5f),
  new Normalizer(0, 1, 1.0f)
};

testing(valuesInRange) {
  static unsigned long startTime = millis();
  static int currentRandom = 0;
  static float currentSign = 1;

  Plaquette.step();

  float randomValue = currentRandom * currentSign;
  currentRandom = (currentRandom + 1) % 101;
  currentSign = -currentSign;

  for (int i=0; i<N_UNITS; i++) {
    Normalizer* unit =(Normalizer*) units[i];
    float value = unit->put(randomValue);

    if (millis() - startTime > 1000) {
//      Serial.printf("val=%f ==> %f\n", randomValue, value );
      assertMoreOrEqual(value, -3.1f);
      assertLessOrEqual(value,  3.1f);

      assertEqual(unit->isLowOutlier(-100, 1.0f), true);
      assertEqual(unit->isHighOutlier(100, 1.0f), true);
      assertEqual(unit->isOutlier(-100, 1.0f), true);
      assertEqual(unit->isOutlier(-100, 1.0f), true);
    }

    // if (millis() - startTime > 4000) {
    //   assertNear(unit->normalize(100),   1.0f, 0.5f);
    //   assertNear(unit->normalize(-100), -1.0f, 0.5f);
    // }
  }
  if (millis() - startTime > 5000) pass();
}

void setup() {
  Plaquette.begin();
// Plaquette.sampleRate(20000);
}

void loop() {
  aunit::TestRunner::run();
}
