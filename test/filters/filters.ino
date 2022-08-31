#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>

using namespace pq;

#define N_UNITS 1
Node* units[N_UNITS] = {
  // new MinMaxScaler(0.01f),
  new MinMaxScaler(1.0f)
};

// testing(nSteps) {
//   static int nSteps = (int)Plaquette.nSteps();
//   Plaquette.step();
//   assertEqual((int)Plaquette.nSteps(), nSteps);
//   nSteps ++;
//   if (nSteps > 100) pass();
// }

testing(valuesIn01) {
  static unsigned long startTime = millis();

  Plaquette.step();
  float randomValue = randomFloat(-100, 100);

  for (int i=0; i<N_UNITS; i++) {
    MinMaxScaler* unit =(MinMaxScaler*) units[i];
    float value = unit->put(randomValue);
    // Serial.printf("val=%f min=%f max=%f ==> %f\n", randomValue, unit->min(), unit->max(), value );
    assertMoreOrEqual(value, 0.0f);
    assertLessOrEqual(value, 1.0f);
  }
  if (millis() - startTime > 1000) pass();
}

testing(smoothing) {
  static float startTime = -1;
  static boolean started = false;

  Plaquette.step();
  if (startTime <= 0)
    startTime = seconds();
  if (!started) {
    for (int i=0; i<N_UNITS; i++) {
      MinMaxScaler* unit =(MinMaxScaler*) units[i];
      unit->put(-100);
      unit->put(100);
    }
    started = true;
  }
  else {
    float t = seconds() - startTime;
    for (int i=0; i<N_UNITS; i++) {
      MinMaxScaler* unit =(MinMaxScaler*) units[i];
      float value = unit->put(0);
      assertMoreOrEqual(unit->min(), -100.0f);
      assertLessOrEqual(unit->max(),  100.0f);
      assertLess(abs(unit->max() + unit->min()), 50.0f);
      // Serial.printf("val=%f min=%f max=%f ==> %f\n", value, unit->min(), unit->max(), value );
      if (t >= unit->time()*2) {
        assertMoreOrEqual(unit->min(),  -50.0f);
        assertLessOrEqual(unit->max(),   50.0f);
      }
    }
    if (t > 1) pass();
  }
}

test(nUnits) {
  assertEqual((int)Plaquette.nUnits(), N_UNITS);
}

void setup() {
  Plaquette.begin();
  Plaquette.sampleRate(1000);

}
void loop() {
  aunit::TestRunner::run();
}
