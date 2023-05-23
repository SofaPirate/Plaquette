#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>

#include "Callback.h"

using namespace pq;

class DummySensor {
  public:
  DummySensor() : _value(0) {}

  float getValue() { 
    return _value;
  }

  void setValue(float x) {
    _value = x;
  }

  float _value;
};

DummySensor sensor1;
DummySensor sensor2;

float dummyRead(DummySensor& s) {
  return s.getValue();
}

void dummyWrite(DummySensor& s, float x) {
  return s.setValue(x);
}

AnalogCallback<DummySensor> analogCallback1(sensor1, dummyRead, dummyWrite);
AnalogCallback<DummySensor> analogCallback2(sensor2, dummyRead, dummyWrite);

testing(analogCallbacks) {
  static unsigned long startTime = millis();
  static float val = 0;


  analogCallback1.put(val);
  analogCallback2.put(val);
  Plaquette.step();

  assertEqual(analogCallback1.get(), val);
  assertEqual(analogCallback2.get(), val);

  val += 0.1f;

  if (millis() - startTime > 1000) pass();
}

void setup() {
  Plaquette.begin();
  Plaquette.sampleRate(1000);
}

void loop() {
  aunit::TestRunner::run();
}
