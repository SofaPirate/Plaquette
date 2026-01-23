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

test(integerScalarArithmetic) {
  Integer v = 10;

  // Value * scalar
  assertEqual(v * 3, 30);
  // scalar * Value
  assertEqual(3 * v, 30);

  // Value / scalar
  assertEqual(v / 2, 5);
  // scalar / Value
  assertEqual(100 / v, 10);

  // Value + scalar (already had overloads, but verify)
  assertEqual(v + 5, 15);
  assertEqual(5 + v, 15);

  // Value - scalar
  assertEqual(v - 3, 7);
  assertEqual(20 - v, 10);
}

test(floatScalarArithmetic) {
  Float v = 2.5f;

  assertNear(v * 2.0f, 5.0f, 0.001f);
  assertNear(2.0f * v, 5.0f, 0.001f);
  assertNear(v / 2.0f, 1.25f, 0.001f);
  assertNear(5.0f / v, 2.0f, 0.001f);
}

test(integerModulo) {
  Integer v = 10;

  // Value % scalar
  assertEqual((int)(v % 3), 1);
  // scalar % Value
  assertEqual((int)(17 % v), 7);
  // Value % Value
  Integer m = 4;
  assertEqual((int)(v % m), 2);

  // Compound modulo assignment
  v %= 3;
  assertEqual((int)v, 1);

  v = 10;
  v %= m;
  assertEqual((int)v, 2);
}

test(unaryNegation) {
  Integer i = 5;
  assertEqual((int)(-i), -5);

  Float f = 2.5f;
  assertNear((float)(-f), -2.5f, 0.001f);

  // Verify original unchanged
  assertEqual((int)i, 5);
  assertNear((float)f, 2.5f, 0.001f);
}

void setup() {
  Plaquette.begin();
}

void loop() {
  aunit::TestRunner::run();
}
