#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>

using namespace pq;

float randomNormal(float mean=0, float stdv=1) {
  static bool normal_is_valid = false;
  static float normal_x;
  static float normal_y;
  static float normal_rho;
  if (!normal_is_valid) {
    normal_x = randomUniform();
    normal_y = randomUniform();
    normal_rho = sqrt(-2. * log(1.0 - normal_y));
    normal_is_valid = true;
  } else
    normal_is_valid = false;

  if (normal_is_valid)
    return normal_rho * cos(2. * PI * normal_x) * stdv + mean;
  else
    return normal_rho * sin(2. * PI * normal_x) * stdv + mean;
}

#define N_UNITS 3
Node* units[N_UNITS] = {
  new Normalizer(0, 1),
  new Normalizer(0, 1, 0.5f),
  new Normalizer(0, 1, 1.0f)
};
//
// testing(valuesInRange) {
//   static unsigned long startTime = millis();
//   static bool initialized = false;
//
//   if (!initialized) {
//     for (int i=0; i<N_UNITS; i++) {
//       Normalizer* unit =(Normalizer*) units[i];
//       unit->reset();
//     }
//   }
//
//   Plaquette.step();
//
//   float randomValue = randomNormal();
//
//   for (int i=0; i<N_UNITS; i++) {
//     Normalizer* unit =(Normalizer*) units[i];
//     float value = unit->put(randomValue);
//
//     if (millis() - startTime > 1000) {
//       unit->noClamp();
//      // Serial.printf("val=%f ==> %f\n", randomValue, value );
//       assertMoreOrEqual(value, -3.3f);
//       assertLessOrEqual(value,  3.3f);
//
//       assertEqual(unit->isLowOutlier(-100, 1.0f), true);
//       assertEqual(unit->isHighOutlier(100, 1.0f), true);
//       assertEqual(unit->isOutlier(-100, 1.0f), true);
//       assertEqual(unit->isOutlier(-100, 1.0f), true);
//     }
//   }
//   if (millis() - startTime > 5000) pass();
// }

testing(matchRandomNormal) {
  static unsigned long startTime = millis();
  static bool initialized = false;

  if (!initialized) {
    for (int i=0; i<N_UNITS; i++) {
      Normalizer* unit =(Normalizer*) units[i];
      unit->reset();
    }
    initialized = true;
  }

  Plaquette.step();

  float randomValue = randomNormal();

  for (int i=0; i<N_UNITS; i++) {
    Normalizer* unit =(Normalizer*) units[i];
    unit->put(randomValue);
    unit->put(-randomValue);
  }
  if (millis() - startTime > 5000) {
    for (int i=0; i<N_UNITS; i++) {
      Normalizer* unit =(Normalizer*) units[i];
      unit->noClamp();
      assertNear(unit->mean(), 0.0f, 0.2f);
      assertNear(unit->stddev(), 1.0f, 0.2f);
      unit->clamp();
      unit->targetMean(NORMALIZER_DEFAULT_MEAN);
      unit->targetStdDev(NORMALIZER_DEFAULT_STDDEV);
      assertNear(unit->put(1000), 1.0f, FLT_MIN);
      assertNear(unit->put(-1000), 0.0f, FLT_MIN);
    }
    pass();
  }
}

void setup() {
  Plaquette.begin();
// Plaquette.sampleRate(20000);
}

void loop() {
  aunit::TestRunner::run();
}
