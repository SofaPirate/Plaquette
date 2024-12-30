#include <Arduino.h>
#include <PlaquetteLib.h>

using namespace pq;

PeakDetector detector(0, PEAK_MAX);

void setup() {
  Plaquette.begin();
}

void loop() {
  Plaquette.step();
  Serial.println("Test");
}
