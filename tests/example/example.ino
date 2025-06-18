#include <Arduino.h>
#include <PlaquetteLib.h>

using namespace pq;

//PeakDetector detector(0, PEAK_MAX);
TriangleWave wave = TriangleWave(1);
TriangleWave wave2 = wave;
//TriangleWave waves[2] = {1,2};

void setup() {
  Plaquette.begin();
  Serial.println(Plaquette.nUnits());
  wave.frequency(2);
  wave2.frequency(4);
}

void loop() {
  Plaquette.step();
  Serial.print(wave);
  round(1.5);
  Serial.print(" ");
  Serial.println(wave2);
}
