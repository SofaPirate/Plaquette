#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>

using namespace pq;

#define N_UNITS 2
Unit* units[N_UNITS] = {
 new MinMaxScaler{},
 new MinMaxScaler(1.0f),
};

#define N_ROBUST_SCALERS 8
RobustScaler* robusts[N_ROBUST_SCALERS];
float quantileLevels[N_ROBUST_SCALERS/2] = { 0.01, 0.1, 0.25, 0.4 };
MinMaxScaler basic;

Smoother smoother(0.5f);

test(basic) {
  Plaquette.step();

  basic.put(-100);
  assertEqual(basic.minValue(), -100.0f);
  Plaquette.step();

  // Check that value is moving towards max.
  basic.put(+100);
  float prevMax = basic.maxValue();
  while (basic.maxValue() < 100.0f) {
    Plaquette.step();
    assertMore(basic.maxValue(), prevMax);
    prevMax = basic.maxValue();
  }
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

#define N_RANDOM_NUMBERS 1000000UL
test(robustScaler) {
  Plaquette.step();
  // robust.reset(0, 1);
  for (uint32_t i=0; i<N_RANDOM_NUMBERS; i++) {
    float randomValue = randomFloat();
    for (int i=0; i<N_ROBUST_SCALERS; i++) {
      robusts[i]->put(randomValue);
    }
    Plaquette.step();
  }
  // Serial.println(robust.stdDev());

  for (int i=0; i<N_ROBUST_SCALERS; i++) {
    assertNear(robusts[i]->lowQuantile(), robusts[i]->lowQuantileLevel(), 0.05f);
    assertNear(robusts[i]->highQuantile(), robusts[i]->highQuantileLevel(), 0.05f);
  }
}

testing(valuesIn01) {
  static unsigned long startTime = millis();

  Plaquette.step();
  float randomValue = randomFloat(-100, 100);

  for (int i=0; i<N_UNITS; i++) {
    Unit* filter = units[i];
    float value = filter->put(randomValue);
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
  assertNear(smoother.get(), 1.25f, 0.01f);
  Plaquette.step();
  assertNear(smoother.get(), 1.25f, 0.01f);
  Plaquette.step();
  assertNear(smoother.get(), 1.33f, 0.01f);
}

void setup() {
  Plaquette.begin();
  for (int i=0; i<N_ROBUST_SCALERS; i+=2) {
    robusts[i] = new RobustScaler();
    robusts[i+1] = new RobustScaler(0.5f);
    robusts[i]->lowQuantileLevel(quantileLevels[i]);
    robusts[i+1]->lowQuantileLevel(quantileLevels[i]);
  }
}

void loop() {
  aunit::TestRunner::run();
}
