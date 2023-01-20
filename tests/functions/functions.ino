#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>
#include <pq_random.h>

#include <pq_osc_utils.h>

using namespace pq;
using namespace aunit;

test(wrapInt) {
  assertEqual(wrap(1,   3),  1.0f);
  assertEqual(wrap(1,  -3),  -2.0f);

  assertEqual(wrap(1, -3, 3),   1.0f);
  assertEqual(wrap(3, -3, 3),  -3.0f);
  assertEqual(wrap(4, -3, 3),  -2.0f);
  assertEqual(wrap(-4, -3, 3),  2.0f);
}

test(wrapBase) {
  assertNear(wrap(0.3,  2.0),  0.3, 0.0001);
  assertNear(wrap(0.3, -2.0), -1.7, 0.0001);

  assertNear(wrap(0.3,  -1.1,  1.1),  0.3, 0.0001);
  assertNear(wrap(1.3,  -1.1,  1.1), -0.9, 0.0001);
  assertNear(wrap(-1.3, -1.1,  1.1),  0.9, 0.0001);
}

test(wrap01) {
  for (float frac = 0; frac < 1; frac += 0.0001)
    for (float f = -5.0f; f < 5.0f; f += 1.0f)
      assertNear(wrap(f+frac),    frac, 0.0001);

  for (float f = -2.5f; f <= 2.5f; f += 0.1f) {
    assertEqual(wrap(f, 1),    wrap(f));
    assertEqual(wrap(f, 0, 1), wrap(f));
  }
}

test(wrapHighPositiveNegative) {
  for (float high=1; high<5; high+=0.1f)
    for (float f = 0; f<high; f+=0.01) {
      assertNear(wrap(f, high), f, 0.0001f);
      assertNear(wrap(f, -high), -high+f, 0.0001f);
    }
}

test(wrapHighLowSwap) {
  for (float low=-5; low<5; low+=0.1f)
    for (float high=-5; high<5; high+=0.1f)
      for (float f=-5; f<5; f+=0.01f)
        assertNear(wrap(f, low, high), wrap(f, high, low), 0.0001);
}

void setup() {
  Plaquette.begin();
  Plaquette.sampleRate(10000);
}

void loop() {
  aunit::TestRunner::run();
}
