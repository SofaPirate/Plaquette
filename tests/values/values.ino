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

// Test scalar comparisons (fixes ambiguous overload issue)
test(integerScalarComparisons) {
  Integer v = 10;

  // Value > scalar
  assertTrue(v > 5);
  assertFalse(v > 15);
  assertFalse(v > 10);

  // Value < scalar
  assertTrue(v < 15);
  assertFalse(v < 5);
  assertFalse(v < 10);

  // Value >= scalar
  assertTrue(v >= 10);
  assertTrue(v >= 5);
  assertFalse(v >= 15);

  // Value <= scalar
  assertTrue(v <= 10);
  assertTrue(v <= 15);
  assertFalse(v <= 5);

  // scalar > Value
  assertTrue(15 > v);
  assertFalse(5 > v);

  // scalar < Value
  assertTrue(5 < v);
  assertFalse(15 < v);

  // scalar >= Value
  assertTrue(10 >= v);
  assertTrue(15 >= v);
  assertFalse(5 >= v);

  // scalar <= Value
  assertTrue(10 <= v);
  assertTrue(5 <= v);
  assertFalse(15 <= v);
}

test(floatScalarComparisons) {
  Float v = 2.5f;

  assertTrue(v > 2.0f);
  assertFalse(v > 3.0f);
  assertTrue(v < 3.0f);
  assertFalse(v < 2.0f);
  assertTrue(v >= 2.5f);
  assertTrue(v <= 2.5f);

  // Reverse direction
  assertTrue(3.0f > v);
  assertTrue(2.0f < v);
}

test(valueToValueComparisons) {
  Integer a = 10;
  Integer b = 20;
  Integer c = 10;

  assertTrue(a < b);
  assertTrue(b > a);
  assertTrue(a <= c);
  assertTrue(a >= c);
  assertTrue(a == c);
  assertFalse(a != c);
  assertTrue(a != b);
}

void setup() {
  Plaquette.begin();
}

void loop() {
  aunit::TestRunner::run();
}
