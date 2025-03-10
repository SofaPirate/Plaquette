#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>

using namespace pq;

#define N_METRO 3
Metronome* metro[N_METRO] = {
  new Metronome(0.01),
  new Metronome(0.1),
  new Metronome(0.1)
};

SquareWave* osc[N_METRO] = {
  new SquareWave(0.01),
  new SquareWave(0.1),
  new SquareWave(0.1)
};

#define TEST_RAMP_MAX 100.0f
#define TEST_RAMP_DURATION 5.0f
#define TEST_RAMP_SPEED (TEST_RAMP_MAX/TEST_RAMP_DURATION)

Ramp testRampDuration;
Ramp testRampSpeed;


testing(timing) {
  static float startTime = Plaquette.seconds();

  static float nMetro[N_METRO] = { 0.0f, 0.0f, 0.0f };
  // static float nMetro[N_METRO] = { 0.0f , 0.0f};
  // static float nMetro[N_METRO] = { 0.0f };
  static const float TOTAL_DURATION = 5.0f;

  Plaquette.step();

  for (int i=0; i<N_METRO; i++) {
    Metronome* unit = metro[i];
    SquareWave *unit2 = osc[i];
    // if (i == 0) {
    //   print(Plaquette.seconds()-startTime); print(" "); print(unit->isOn()); print(" "); print(unit2->get());
    //   if (unit->isOn())
    //     print(" ***");
    //   println();
    // }
    if (unit->isOn()) {
      nMetro[i]++;
      if (i != 2)
        assertEqual(unit2->get(), 1.0f);
    }
  }

  if (Plaquette.seconds() <= TEST_RAMP_MAX) {
    assertMoreOrEqual(testRampDuration.get(), 0.0f);
    assertLessOrEqual(testRampDuration.get(), 100.0f);

    assertNear(testRampDuration.get(), mapFrom01((Plaquette.seconds()-startTime)/TEST_RAMP_DURATION, 0, TEST_RAMP_MAX), 0.1f);
    assertEqual(testRampDuration.get(), testRampSpeed.get());
  }

  if (Plaquette.seconds() - startTime > TOTAL_DURATION) {
    for (int i=0; i<N_METRO; i++) {
      Metronome* unit = metro[i];
      assertNear(nMetro[i], (float)(TOTAL_DURATION/unit->period()), 4.0f);
      pass();
    }
  }
}

void setup() {
  Plaquette.begin();
  for (int i=0; i<N_METRO; i++) {
    SquareWave *unit2 = osc[i];
    unit2->phase(-0.25f);
    unit2->width(0.75f);
  }

  testRampDuration.go(0, TEST_RAMP_MAX, TEST_RAMP_DURATION);
  testRampSpeed.mode(RAMP_SPEED);
  testRampSpeed.go(0, TEST_RAMP_MAX, TEST_RAMP_SPEED);

  Plaquette.sampleRate(100000.0f);
}

void loop() {
  aunit::TestRunner::run();
}
