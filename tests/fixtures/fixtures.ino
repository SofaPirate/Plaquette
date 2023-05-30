#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>

#include "Fixtures.h"

using namespace pq;

float dummyVal = 0;

class DummySensor {
  public:
  DummySensor() {}

  void init() {
  }

  float getValue() { 
    return dummyVal;
  }
};

class DummyButton {
public:
  DummyButton() :_value(false) {}

  void update() {
    _value = !_value; 
  }

  bool get() const { return _value; }

  bool _value;
};

DummySensor sensor1;
DummySensor sensor2;

DummyButton button1;

float dummyRead() {
  return dummyVal;
}

void dummyWrite(float x) {
  dummyVal = x;
}

bool prevButtonValue = false;

AnalogCallbacks<DummySensor> dummySensorCallbacks(
  [](DummySensor& s) -> float { return s.getValue(); },
  [](DummySensor& s) -> void { s.init(); }
);

AnalogFixture<DummySensor> analogSensor1(sensor1, dummySensorCallbacks);
AnalogFixture<DummySensor> analogSensor2(sensor2, dummySensorCallbacks);
AnalogFixture<> analogSensor3(dummyRead);

DigitalFixture<DummyButton> digitalSensor1(button1, 
                                            [](DummyButton& b) -> bool { return b.get(); }, 
                                            NO_CALLBACK, 
                                            NO_CALLBACK, 
                                            [](DummyButton& b) -> void { b.update(); });

testing(analogSensors) {
  static unsigned long startTime = millis();
  static float val = 0;

  analogSensor1.smooth();

  prevButtonValue = digitalSensor1.isOn();
  Plaquette.step();

//  assertEqualNear(analogSensor1.get(), dummyVal);
  assertEqual(analogSensor2.get(), dummyVal);
  assertEqual(analogSensor3.get(), dummyVal);

  assertEqual(digitalSensor1.isOn(), !prevButtonValue);

  val += 0.1f;
  dummyVal++;

  if (millis() - startTime > 1000) pass();
}

void setup() {
  Plaquette.begin();
  Plaquette.sampleRate(1000);
}

void loop() {
  aunit::TestRunner::run();
}
