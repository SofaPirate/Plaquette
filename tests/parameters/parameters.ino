#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>

using namespace pq;

Wave squareWave(SQUARE);

test(parameters) {
  10 >> squareWave.period();
  assertEqual(10.0f, squareWave.period());
}

test(reference) {
  Wave::Parameter period = squareWave.period();
  Wave::Parameter frequency = squareWave.frequency();
  for (float p = 1; p < 10; p ++) {
    p >> period;
    assertNear(period, frequencyToPeriod(frequency), 0.00001);
  }
}

auto period = squareWave.period();
auto frequency = squareWave.frequency();
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
