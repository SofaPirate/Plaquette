#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>

using namespace pq;

#define N_UNITS 2
Unit* units[N_UNITS] = {
  new MinMaxScaler(),
  new MinMaxScaler(1.0f)
};

MinMaxScaler basic;

Smoother smoother(0.5);

test(basic) {
  Plaquette.step();

  basic.put(-100);
  assertEqual(basic.minValue(), -100.0f);
  Plaquette.step();

  basic.put(+100);
  assertEqual(basic.maxValue(), 100.0f);
  Plaquette.step();

  for (float f = -100; f<=100; f++) {
    assertNear(basic.put(f), mapFloat(f, -100, 100, 0, 1), 0.01f);
    Plaquette.step();
  }

}

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

      assertMoreOrEqual(unit->minValue(), -100.0f);
      assertLessOrEqual(unit->maxValue(),  100.0f);

      assertLess(abs(unit->maxValue() + unit->minValue()), 50.0f);

      if (!unit->timeWindowIsInfinite() &&
          t >= unit->timeWindow()*2) {
        assertMoreOrEqual(unit->minValue(),  -50.0f);
        assertLessOrEqual(unit->maxValue(),   50.0f);
      }
    }
    if (t > 1) pass();
  }
}

test(smoother) {
  smoother.reset();
  1 >> smoother;
  assertEqual(smoother.get(), 1.0f);
  Plaquette.step();
  assertEqual(smoother.get(), 1.0f);
  1 >> smoother;
  assertEqual(smoother.get(), 1.0f);
  2 >> smoother;
  assertNear(smoother.get(), 1.17f, 0.01f);
  Plaquette.step();
  assertNear(smoother.get(), 1.17f, 0.01f);
  Plaquette.step();
  assertNear(smoother.get(), 1.28f, 0.01f);
  Plaquette.step();
  assertNear(smoother.get(), 1.33f, 0.01f);
}

void setup() {
  Plaquette.begin();
  Plaquette.sampleRate(1000);
}

void loop() {
  aunit::TestRunner::run();
}
