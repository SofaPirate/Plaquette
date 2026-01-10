#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>

using namespace pq;

Wave squareWave(SQUARE);

Ramp ramp(10);
AbstractTimer* timer = &ramp;

test(parameters) {
  10 >> squareWave.Period();
  assertEqual(10.0f, squareWave.period());
}

test(virtualParameters) {
  2 >> ramp.Duration();
  assertEqual(2.0f, ramp.duration());
  assertEqual(ramp.mode(), RAMP_DURATION);

  2 >> ramp.Speed();
  assertEqual(2.0f, ramp.speed());
  assertEqual(ramp.mode(), RAMP_SPEED);

  2 >> timer->Duration();
  assertEqual(2.0f, ramp.duration());
  assertEqual(2.0f, timer->duration());
  assertEqual(ramp.mode(), RAMP_DURATION);
}

test(reference) {
  Wave::Parameter period = squareWave.Period();
  Wave::Parameter frequency = squareWave.Frequency();
  for (float p = 1; p < 10; p ++) {
    p >> period;
    assertNear(period, frequencyToPeriod(frequency), 0.00001);
  }
}

auto period = squareWave.Period();
auto frequency = squareWave.Frequency();
testing(global) {
  (period + 1) >> period;
  assertNear(period, frequencyToPeriod(frequency), 0.00001);
  if (period > 10)
    pass();
}

void setup() {
  Plaquette.begin();
}

void loop() {
  aunit::TestRunner::run();
}
