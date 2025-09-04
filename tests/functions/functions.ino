#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>
#include <pq_random.h>

#include <pq_phase_utils.h>

using namespace pq;
using namespace aunit;

test(map) {

  for (uint8_t mode=0; mode<=2; mode++) {
    assertEqual(mapFrom01(0, -10, 10, mode), -10.0);
    assertEqual(mapFrom01(1, -10, 10, mode),  mode == WRAP ? -10: 10.0);

    assertEqual(mapTo01(-10, -10, 10, mode), 0.0);
    assertEqual(mapTo01( 10, -10, 10, mode), mode == WRAP ? 0.0 : 1.0);

    assertEqual(mapFloat(-10, -10, 10, -20, 20, mode), -20.0);
    assertEqual(mapFloat( 10, -10, 10, -20, 20, mode), mode == WRAP ? -20.0 : 20.0);
  }

  assertEqual(mapFloat(-20, -10, 10, -20, 20, UNCONSTRAIN), -40.0);
  assertEqual(mapFloat(-20, -10, 10, -20, 20, CONSTRAIN), -20.0);
  assertEqual(mapFloat(-20, -10, 10, -20, 20, WRAP), 0.0);
}

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
      assertNear(wrap01(f+frac),    frac, 0.0001);

  for (float f = -2.5f; f <= 2.5f; f += 0.1f) {
    assertEqual(wrap(f, 1),    wrap01(f));
    assertEqual(wrap(f, 0, 1), wrap01(f));
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

uint32_t countRandomTrigger(int randomTimeBlock, int nBlocks) {
  uint32_t count = 0;
  for (int i=0; i<nBlocks; i++) {

    unsigned long startTime = micros();
    unsigned long prevTime = startTime;

    while (true) {
      unsigned long now = micros();
      unsigned long samplePeriod = now - prevTime;

      if (randomTrigger(randomTimeBlock, samplePeriod)) {
        count++;
      }

      prevTime = now;
      if (now - startTime > randomTimeBlock)
        break;
    }
  }

  return count;
}

test(random) {
  #define BASE_N_BLOCKS 1000
  #define LOW_N_BLOCKS 100
  #define TOLERANCE 0.05f
  assertNear((float)countRandomTrigger(10, BASE_N_BLOCKS), (float)BASE_N_BLOCKS, BASE_N_BLOCKS*TOLERANCE);
  assertNear((float)countRandomTrigger(100, BASE_N_BLOCKS), (float)BASE_N_BLOCKS, BASE_N_BLOCKS*TOLERANCE);
  assertNear((float)countRandomTrigger(1000, BASE_N_BLOCKS), (float)BASE_N_BLOCKS, BASE_N_BLOCKS*TOLERANCE);
  assertNear((float)countRandomTrigger(10000, LOW_N_BLOCKS), (float)LOW_N_BLOCKS, LOW_N_BLOCKS*TOLERANCE);
  assertNear((float)countRandomTrigger(100000, LOW_N_BLOCKS), (float)LOW_N_BLOCKS, LOW_N_BLOCKS*TOLERANCE);
  assertNear((float)countRandomTrigger(1000000, LOW_N_BLOCKS), (float)LOW_N_BLOCKS, LOW_N_BLOCKS*TOLERANCE);
}

void setup() {
  Plaquette.begin();
}

void loop() {
  aunit::TestRunner::run();
}
