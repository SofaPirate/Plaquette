#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>
#include "PeakDetector.h"

using namespace pq;

#define N_GENERATORS 5
Node* generators[N_GENERATORS] = {
  new SquareOsc(1),
  new TriOsc(1, 0.5),
  new TriOsc(1, 0),
  new TriOsc(1, 1),
  // new TriOsc(0),
  // new TriOsc(1),
  new SineOsc(1)
};

#define N_DETECTOR_TYPES 4
PeakDetector* detectors[N_DETECTOR_TYPES][N_GENERATORS];

PeakDetector slowCrossingDetector(128, PEAK_MAX);
PeakDetector justOverThresholdDetector(128, PEAK_MAX);

testing(countingPeaks) {
  static unsigned long startTime = millis();
  static uint16_t nPeaks = 0;

  static uint16_t nSlowCrossingPeaks = 0;
  static uint16_t nJustOverThresholdPeaks = 0;

  Plaquette.step();

  for (int i=0; i<N_GENERATORS; i++) {
    Node* unit = generators[i];
    float value = unit->get();

    for (int j=0; j<N_DETECTOR_TYPES; j++) {
      PeakDetector* detector = detectors[j][i];

      value >> *detector;

      // print("Detector: "); println(detector->isOn());

      if (detector->isOn()) {
        nPeaks ++;
        // print("crossing : "); println(value);
        assertNear(detector->get(), 1.0f, 0.1f);
      }
    }
  }

  // Generates oscillation between 0 and 255.
  uint8_t slowCrossingSignal = (uint8_t)mapFloat(millis(), 0, 1000, 0, 255, WRAP);
  slowCrossingSignal >> slowCrossingDetector;
  if (slowCrossingDetector.isOn()) {
    nSlowCrossingPeaks++;
  }

  // Generates oscillation between 127 and 128.
  uint8_t justOverThresholdSignal = millis() % 1000 < 500 ? 128 : 127;

  justOverThresholdSignal >> justOverThresholdDetector;
  if (justOverThresholdDetector.isOn()) {
    nJustOverThresholdPeaks++;
  }

  if (millis() - startTime > 5000) {
    assertNear(nPeaks, (uint16_t)(5*N_GENERATORS*N_DETECTOR_TYPES), 15);
    assertNear(nSlowCrossingPeaks, 5, 1);
    assertNear(nJustOverThresholdPeaks, 5, 1);
    pass();
  }
}

void setup() {
  Plaquette.begin();
  Plaquette.sampleRate(20000);

  for (int i=0; i<N_DETECTOR_TYPES; i++)
    for (int j=0; j<N_GENERATORS; j++) {
      detectors[i][j] = new PeakDetector(0.5, (uint8_t)(i));
    }
}

void loop() {
  aunit::TestRunner::run();
}
