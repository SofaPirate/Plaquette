#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>

using namespace pq;

#define N_GENERATORS 6
Osc* generators[N_GENERATORS] = {
  new SquareOsc(0.005),
  new SquareOsc(1),
  new TriOsc(0.005),
  new TriOsc(1),
  new SineOsc(0.005),
  new SineOsc(1)
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
    Node* unit = generators[i];
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
      Osc* unit = generators[i];
      // Serial.println(unit->period());
      assertNear((float)nPeaks[i], RUNTIME / unit->period(), 1 / (unit->period() * unit->period()));
    }
    pass();
  }
}

test(nUnits) {
  assertEqual((int)Plaquette.nUnits(), N_GENERATORS*2);
}

void setup() {
  Plaquette.begin();
  // Plaquette.sampleRate(10000);
  for (int i=0; i<N_GENERATORS; i++) {
    detectors[i]->reloadThreshold(PEAK_DETECTOR_THRESHOLD-0.1f);
  }
}

void loop() {
  aunit::TestRunner::run();
}
