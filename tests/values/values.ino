#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>

using namespace pq;

Integer intValue   = 10;
Float   floatValue = 20.0f;
Boolean boolValue  = true;

test(integer) {
  assertEqual(intValue, 10.0f);
  intValue++;
  assertEqual(intValue, 11.0f);
  int value = intValue;
  assertEqual(value, 11);
}

test(floating) {
  assertEqual(floatValue, 20.0f);
  floatValue += 2;
  assertEqual(floatValue, 22.0f);
}

test(boolean) {
  assertEqual(boolValue, 1.0f);
  bool value = boolValue;
  assertEqual(value, true);
  !boolValue >> boolValue;
  assertEqual(boolValue, 0.0f);
}

void setup() {
  Plaquette.begin();
}

void loop() {
  aunit::TestRunner::run();
}
