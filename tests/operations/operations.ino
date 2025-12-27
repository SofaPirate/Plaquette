#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>
#include <pq_random.h>

using namespace pq;
using namespace aunit;

DigitalOut dOut1(0);
DigitalOut dOut2(1);
AnalogOut aOut1(9);
AnalogOut aOut2(10);
AnalogOut aOut3(11);

test(digital) {
  Plaquette.step();
  dOut1.off();
  dOut2.on();
  assertEqual(dOut1, false);
  assertEqual(dOut2, true);

  if (!dOut1) pass();
  if (dOut2) pass();

  assertEqual(!dOut1, true);
  assertEqual(!dOut2, false);
  assertEqual(dOut1 && dOut2, false);
  assertEqual(dOut1 || dOut2, true);
  assertEqual(!dOut1 && dOut2, true);
  assertEqual(dOut1 || !dOut2, false);

  assertEqual(dOut1 * dOut2, 0);
  assertEqual(dOut1 / dOut2, 0);

  Plaquette.step();
  dOut1.toggle();
  dOut2.toggle();
  assertEqual(dOut1.changed(), true);
  assertEqual(dOut1.rose(), true);
  assertEqual(dOut1.fell(), false);
  assertEqual(dOut2.changed(), true);
  assertEqual(dOut2.rose(), false);
  assertEqual(dOut2.fell(), true);
}

test(analog) {
  Plaquette.step();
  aOut1.put(0);
  aOut2.put(1);
  dOut1.off();
  dOut2.on();

  assertEqual(1+dOut1, 1);
  assertEqual(1-dOut2, 0);
  assertEqual(1+aOut1, 1.0f);
  assertEqual(1-aOut2, 0.0f);

  assertEqual(dOut2/2, 0);
  assertEqual(aOut2/2, 0.5f);

  assertEqual(2*dOut1, 0);
  assertEqual(2*dOut2, 2);
  assertEqual(2.0f*dOut1, 0.0f);
  assertEqual(2.0f*dOut2, 2.0f);
  assertEqual(2*aOut1, 0.0f);
  assertEqual(2*aOut2, 2.0f);
  assertEqual(2.0f*aOut1, 0.0f);
  assertEqual(2.0f*aOut2, 2.0f);
  assertEqual(2.0*aOut1, 0.0f);
  assertEqual(2.0*aOut2, 2.0f);

  assertEqual(dOut1*2, 0);
  assertEqual(dOut2*2, 2);
  assertEqual(dOut1*2.0f, 0.0f);
  assertEqual(dOut2*2.0f, 2.0f);
  assertEqual(aOut1*2, 0.0f);
  assertEqual(aOut2*2, 2.0f);

  (aOut1+aOut2) >> aOut3;
  assertEqual(aOut3, 1.0f);

  (aOut2-aOut1) >> aOut3;
  assertEqual(aOut3, 1.0f);

  (aOut1*aOut2) >> aOut3;
  assertEqual(aOut3, 0.0f);

  (aOut2/aOut2) >> aOut3;
  assertEqual(aOut3, 1.0f);
}

test(math) {
  // --- min / max ---
  assertEqual(min(3, 5), 3);
  assertEqual(max(3, 5), 5);
  assertEqual(min(2.5f, 1.5f), 1.5f);
  assertEqual(max(2.5, 3.1), 3.1);

  assertEqual(min(3, 2.5), 2.5); // mixed types
  assertEqual(max(3, 2.5f), 3.0f); // mixed types

  // --- abs ---
  assertEqual(abs(-5), 5);
  assertEqual(abs(5), 5);
  assertEqual(abs(-3.2f), 3.2f);

  // --- constrain ---
  assertEqual(constrain(5, 0, 10), 5);
  assertEqual(constrain(-3, 0, 10), 0);
  assertEqual(constrain(15, 0, 10), 10);
  assertEqual(constrain(3.2f, 1.0f, 3.0f), 3.0f);
  assertEqual(constrain(2.5, 0.0, 3.0), 2.5);

  assertEqual(constrain(5, 0.0, 10.0f), 5.0); // mixed types
  assertEqual(constrain(2.5, 3.0, 0.0), 3.0); // strange but consistent with Arduino's version

  // --- round ---
  assertEqual(round(2.3f), 2L);
  assertEqual(round(2.7f), 3L);
  assertEqual(round(-2.3f), -2L);
  assertEqual(round(-2.7f), -3L);

  // --- radians / degrees ---
  assertEqual(radians(180.0), PI); // crude check
  assertEqual(degrees(PI), 180.0);

  // --- sq ---
  assertEqual(sq(3), 9);
  assertEqual(sq(-4), 16);
  assertEqual(sq(1.5), 2.25);
}

test(fixedMath) {
  assertEqual(fixed32ToFloat(0), 0.0f);
  assertEqual(fixed32ToFloat(FIXED_32_MAX), 1.0f);
}

void setup() {
  Plaquette.begin();
}

void loop() {
  aunit::TestRunner::run();
}
