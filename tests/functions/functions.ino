#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>
#include <pq_random.h>

#include <pq_phase_utils.h>
#include <pq_easing.h>

using namespace pq;
using namespace aunit;

// Tolerances for easing function tests.
// fastSin/fastCos use a lookup table approximation.
static const float EASING_SIN_TOL  = 0.01f;
// fastSqrt(0) is non-zero by design (caveat in pq_fastmath.h).
static const float EASING_SQRT_TOL = 0.02f;
// easeOutExpo(1) and easeInOutExpo(1) equal 1-2^-8 and 1-2^-9 by formula design.
static const float EASING_EXPO_TOL = 0.005f;

test(map) {

  for (uint8_t m=0; m<=2; m++) {
    MapMode mode = (MapMode)m;
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
}

// Verifies that all easing functions map t=0 to 0 and t=1 to 1.
test(easingBoundaries) {
  // easeNone: exact identity.
  assertEqual(easeNone(0.0f), 0.0f);
  assertEqual(easeNone(1.0f), 1.0f);

  // Sine family: fastSin/fastCos approximation.
  assertNear(easeInSine(0.0f),     0.0f, EASING_SIN_TOL);
  assertNear(easeInSine(1.0f),     1.0f, EASING_SIN_TOL);
  assertNear(easeOutSine(0.0f),    0.0f, EASING_SIN_TOL);
  assertNear(easeOutSine(1.0f),    1.0f, EASING_SIN_TOL);
  assertNear(easeInOutSine(0.0f),  0.0f, EASING_SIN_TOL);
  assertNear(easeInOutSine(1.0f),  1.0f, EASING_SIN_TOL);

  // Quad through Quint: pure polynomial, exact at endpoints.
  assertEqual(easeInQuad(0.0f),     0.0f);   assertEqual(easeInQuad(1.0f),     1.0f);
  assertEqual(easeOutQuad(0.0f),    0.0f);   assertEqual(easeOutQuad(1.0f),    1.0f);
  assertEqual(easeInOutQuad(0.0f),  0.0f);   assertEqual(easeInOutQuad(1.0f),  1.0f);
  assertEqual(easeInCubic(0.0f),    0.0f);   assertEqual(easeInCubic(1.0f),    1.0f);
  assertEqual(easeOutCubic(0.0f),   0.0f);   assertEqual(easeOutCubic(1.0f),   1.0f);
  assertEqual(easeInOutCubic(0.0f), 0.0f);   assertEqual(easeInOutCubic(1.0f), 1.0f);
  assertEqual(easeInQuart(0.0f),    0.0f);   assertEqual(easeInQuart(1.0f),    1.0f);
  assertEqual(easeOutQuart(0.0f),   0.0f);   assertEqual(easeOutQuart(1.0f),   1.0f);
  assertEqual(easeInOutQuart(0.0f), 0.0f);   assertEqual(easeInOutQuart(1.0f), 1.0f);
  assertEqual(easeInQuint(0.0f),    0.0f);   assertEqual(easeInQuint(1.0f),    1.0f);
  assertEqual(easeOutQuint(0.0f),   0.0f);   assertEqual(easeOutQuint(1.0f),   1.0f);
  assertEqual(easeInOutQuint(0.0f), 0.0f);   assertEqual(easeInOutQuint(1.0f), 1.0f);

  // Expo: fastPow2 is exact at integer exponents.
  // easeOut/InOut at t=1 reach 1-2^-8 and 1-2^-9 by formula design (EASING_EXPO_TOL).
  assertEqual(easeInExpo(0.0f),     0.0f);   assertEqual(easeInExpo(1.0f),     1.0f);
  assertEqual(easeOutExpo(0.0f),    0.0f);   assertNear(easeOutExpo(1.0f),    1.0f, EASING_EXPO_TOL);
  assertEqual(easeInOutExpo(0.0f),  0.0f);   assertNear(easeInOutExpo(1.0f),  1.0f, EASING_EXPO_TOL);

  // Circ: fastSqrt(0) is non-zero, so endpoints near 0 carry EASING_SQRT_TOL.
  assertNear(easeInCirc(0.0f),     0.0f, EASING_SQRT_TOL);
  assertNear(easeInCirc(1.0f),     1.0f, EASING_SQRT_TOL);
  assertNear(easeOutCirc(0.0f),    0.0f, EASING_SQRT_TOL);
  assertNear(easeOutCirc(1.0f),    1.0f, EASING_SQRT_TOL);
  assertNear(easeInOutCirc(0.0f),  0.0f, EASING_SQRT_TOL);
  assertNear(easeInOutCirc(1.0f),  1.0f, EASING_SQRT_TOL);

  // Back: pure polynomial; intentionally overshoots [0,1] in the interior.
  assertEqual(easeInBack(0.0f),    0.0f);   assertEqual(easeInBack(1.0f),    1.0f);
  assertEqual(easeOutBack(0.0f),   0.0f);   assertEqual(easeOutBack(1.0f),   1.0f);
  assertEqual(easeInOutBack(0.0f), 0.0f);   assertEqual(easeInOutBack(1.0f), 1.0f);

  // Elastic: intentionally overshoots [0,1]; boundaries use fastSin.
  assertNear(easeInElastic(0.0f),    0.0f, EASING_SIN_TOL);
  assertNear(easeInElastic(1.0f),    1.0f, EASING_SIN_TOL);
  assertNear(easeOutElastic(0.0f),   0.0f, EASING_SIN_TOL);
  assertNear(easeOutElastic(1.0f),   1.0f, EASING_SIN_TOL);
  assertNear(easeInOutElastic(0.0f), 0.0f, EASING_SIN_TOL);
  assertNear(easeInOutElastic(1.0f), 1.0f, EASING_SIN_TOL);

  // Bounce: sin/cos zeros make endpoints exact, except fastCos(0)≈1 (not exact).
  assertEqual(easeInBounce(0.0f),    0.0f);
  assertNear(easeInBounce(1.0f),     1.0f, EASING_SIN_TOL); // fastSin(3.5π) ≈ ±1
  assertNear(easeOutBounce(0.0f),    0.0f, EASING_SIN_TOL); // fastCos(0) ≈ 1 (not exact)
  assertNear(easeOutBounce(1.0f),    1.0f, EASING_SIN_TOL); // fastCos(3.5π) ≈ 0
  assertEqual(easeInOutBounce(0.0f), 0.0f);
  assertEqual(easeInOutBounce(1.0f), 1.0f); // fastSin(7π) = 0 exactly
}

// Verifies that all InOut easing functions return 0.5 at t=0.5 (symmetry point).
test(easingInOutMidpoint) {
  assertNear(easeInOutSine(0.5f),    0.5f, EASING_SIN_TOL);
  assertEqual(easeInOutQuad(0.5f),   0.5f);
  assertEqual(easeInOutCubic(0.5f),  0.5f);
  assertEqual(easeInOutQuart(0.5f),  0.5f);
  assertEqual(easeInOutQuint(0.5f),  0.5f);
  assertEqual(easeInOutExpo(0.5f),   0.5f);
  assertNear(easeInOutCirc(0.5f),    0.5f, EASING_SQRT_TOL);
  assertEqual(easeInOutBack(0.5f),   0.5f);
  assertNear(easeInOutElastic(0.5f), 0.5f, EASING_SIN_TOL);
  assertNear(easeInOutBounce(0.5f),  0.5f, EASING_SIN_TOL);
}

// Verifies the mirror relationship easeOut(t) == 1 - easeIn(1-t) for polynomial families.
test(easingMirrorSymmetry) {
  for (int i = 1; i <= 9; i++) {
    float t = i * 0.1f;
    float c = 1.0f - t;
    assertNear(easeOutQuad(t),  1.0f - easeInQuad(c),  1e-5f);
    assertNear(easeOutCubic(t), 1.0f - easeInCubic(c), 1e-5f);
    assertNear(easeOutQuart(t), 1.0f - easeInQuart(c), 1e-5f);
    assertNear(easeOutQuint(t), 1.0f - easeInQuint(c), 1e-5f);
    assertNear(easeOutBack(t),  1.0f - easeInBack(c),  1e-5f);
  }
}

// Verifies exact interior values for polynomial easing functions.
test(easingPolynomialValues) {
  // easeNone: linear.
  assertNear(easeNone(0.25f), 0.25f, 1e-5f);
  assertNear(easeNone(0.75f), 0.75f, 1e-5f);

  // easeInQuad: t^2.
  assertNear(easeInQuad(0.25f), 0.0625f,  1e-5f);
  assertNear(easeInQuad(0.5f),  0.25f,    1e-5f);
  assertNear(easeInQuad(0.75f), 0.5625f,  1e-5f);

  // easeOutQuad: t*(2-t) = mirror of easeInQuad.
  assertNear(easeOutQuad(0.25f), 0.4375f, 1e-5f);
  assertNear(easeOutQuad(0.5f),  0.75f,   1e-5f);
  assertNear(easeOutQuad(0.75f), 0.9375f, 1e-5f);

  // easeInCubic: t^3.
  assertNear(easeInCubic(0.25f), 0.015625f, 1e-5f);
  assertNear(easeInCubic(0.5f),  0.125f,    1e-5f);
  assertNear(easeInCubic(0.75f), 0.421875f, 1e-5f);

  // easeOutCubic: 1+(t-1)^3.
  assertNear(easeOutCubic(0.25f), 0.578125f, 1e-5f);
  assertNear(easeOutCubic(0.5f),  0.875f,    1e-5f);
  assertNear(easeOutCubic(0.75f), 0.984375f, 1e-5f);

  // easeInOutCubic: 4t^3 for t<0.5, 1+0.5*(2(t-1))^3 for t>=0.5.
  assertNear(easeInOutCubic(0.25f), 0.0625f,  1e-5f);
  assertNear(easeInOutCubic(0.5f),  0.5f,     1e-5f);
  assertNear(easeInOutCubic(0.75f), 0.9375f,  1e-5f);

  // easeInExpo: (2^(8t)-1)/255 — fastPow2 exact at integer exponents.
  assertEqual(easeInExpo(0.0f), 0.0f);
  assertEqual(easeInExpo(1.0f), 1.0f);

  // easeInBounce / easeOutBounce endpoint checks.
  assertEqual(easeInBounce(0.0f),  0.0f);
  assertNear(easeOutBounce(0.0f),  0.0f, EASING_SIN_TOL); // fastCos(0) ≈ 1 (not exact)
  assertEqual(easeOutBounce(1.0f), 1.0f); // fastCos(3.5π) = 0 cancels fastPow2 term
}

void setup() {
  Plaquette.begin();
}

void loop() {
  aunit::TestRunner::run();
}
