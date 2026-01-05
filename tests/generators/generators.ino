#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>

using namespace pq;

#define N_GENERATORS 6
Wave* generators[N_GENERATORS] = {
  new Wave(0.005),
  new Wave(1),
  new Wave(TRIANGLE, 0.005),
  new Wave(TRIANGLE, 1),
  new Wave(SINE, 0.005),
  new Wave(SINE, 1)
};

#define PEAK_DETECTOR_THRESHOLD 0.6f
PeakDetector* detectors[N_GENERATORS] = {
  new PeakDetector(PEAK_DETECTOR_THRESHOLD),
  new PeakDetector(PEAK_DETECTOR_THRESHOLD),
  new PeakDetector(PEAK_DETECTOR_THRESHOLD),
  new PeakDetector(PEAK_DETECTOR_THRESHOLD),
  new PeakDetector(PEAK_DETECTOR_THRESHOLD),
  new PeakDetector(PEAK_DETECTOR_THRESHOLD)
};

// uint16_t nPeaks[N_GENERATORS] = { 0 };
uint16_t nPeaks[N_GENERATORS] = { 0, 0, 0, 0, 0, 0 };

testing(valuesIn01) {
  static float startTime = -1;
  static const float RUNTIME = 5.0f;
  if (startTime < 0)
    startTime = Plaquette.seconds();

  Plaquette.step();

  for (int i=0; i<N_GENERATORS; i++) {
    Unit* unit = generators[i];
    float value = unit->get();
    value >> *detectors[i];
    assertMoreOrEqual(value, 0.0f);
    assertLessOrEqual(value, 1.0f);

    if (*detectors[i]) {
      nPeaks[i]++;
      // Serial.println(Plaquette.seconds()-startTime);
    }
    // if (i==2) {
    // for (int i=0; i<value*20; i++)
    //   Serial.print(" ");
    // Serial.println("*");
    // }
  }
  if (Plaquette.seconds() - startTime > RUNTIME) {
    for (int i=0; i<N_GENERATORS; i++) {
      AbstractWave* unit = generators[i];
      // Serial.println(unit->period());
      assertNear((float)nPeaks[i], RUNTIME / unit->period(), 1 / (unit->period() * unit->period()));
    }
    pass();
  }
}

Engine engine2;

Wave* waves[N_GENERATORS] = {
  new Wave(TRIANGLE, 1.0, 0, engine2),
  new Wave(TRIANGLE, 1.0, 0.5, engine2),
  new Wave(TRIANGLE, 1.0, 1, engine2),
  new Wave(TRIANGLE, 1.0, 0, engine2),
  new Wave(TRIANGLE, 1.0, 0.5, engine2),
  new Wave(TRIANGLE, 1.0, 1, engine2)
};

int countSkew[N_GENERATORS] = { 0, 0, 0, 0, 0, 0 };
int countPeriod[N_GENERATORS] =  { 0, 0, 0, 0, 0, 0 };

#define TOLERANCE 0.1f
testing(waveEvents) {
  static float startTime = -1;
  static const float RUNTIME = 5.0f;
  if (startTime < 0)
    startTime = engine2.seconds();

  engine2.step();


  for (int i=0; i<N_GENERATORS; i++) {
    Wave* wave = waves[i];

    // Serial.print(i); Serial.print(" "); Serial.println(wave->get());


    if (wave->passedSkew()) {
      float expectedValue;
      if ((wave->isForward() && wave->skew() == 1) || (!wave->isForward() && wave->skew() == 0))
        expectedValue = 0;
      else
        expectedValue = 1;
      assertNear(wave->get(), expectedValue, TOLERANCE);
      countSkew[i]++;
    }
    if (wave->passedPeriod()) {
      float expectedValue;
      if ((wave->isForward() && wave->skew() == 0) || (!wave->isForward() && wave->skew() == 1))
        expectedValue = 1;
      else
        expectedValue = 0;
      assertNear(wave->get(), expectedValue, TOLERANCE);
      countPeriod[i]++;
    }

    if (wave->skew() == 0 || wave->skew() == 1)
      assertEqual((bool)(wave->passedPeriod() ^ wave->passedSkew()), false);
    else
      assertEqual(wave->passedPeriod() && wave->passedSkew(), false);
  }

  if (engine2.seconds() - startTime > RUNTIME) {
    for (int i=0; i<N_GENERATORS; i++) {
      Wave* wave = waves[i];
      assertNear((float)countSkew[i], RUNTIME / wave->period(), 1 / (wave->period() * wave->period()));
    }
    pass();
  }
}

test(nUnits) {
  assertEqual((int)Plaquette.nUnits(), N_GENERATORS*2);
  assertEqual((int)engine2.nUnits(), N_GENERATORS);
}

void setup() {
  Plaquette.begin();
  for (int i=0; i<N_GENERATORS; i++) {
    detectors[i]->reloadThreshold(PEAK_DETECTOR_THRESHOLD-0.1f);
  }

  engine2.begin();
  for (int i=3; i<N_GENERATORS; i++) {
    waves[i]->reverse();
  }
}

void loop() {
  aunit::TestRunner::run();
}
