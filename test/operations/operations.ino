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
}

void setup() {
  Plaquette.begin();
  Plaquette.sampleRate(10000);
}

void loop() {
  aunit::TestRunner::run();
}
