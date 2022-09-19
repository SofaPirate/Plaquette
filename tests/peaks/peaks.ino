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

testing(countingPeaks) {
  static unsigned long startTime = millis();
  static uint16_t nPeaks = 0;

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
 if (millis() - startTime > 5000) {
   assertNear(nPeaks, (uint16_t)(5*N_GENERATORS*N_DETECTOR_TYPES), 1);
   pass();
 }
}

void setup() {
  Plaquette.begin();
  Plaquette.sampleRate(20000);

  for (int i=0; i<N_DETECTOR_TYPES; i++)
    for (int j=0; j<N_GENERATORS; j++) {
      detectors[i][j] = new PeakDetector(0.5, (uint8_t)(i+2));
    }
}

void loop() {
  aunit::TestRunner::run();
}
