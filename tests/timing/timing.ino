#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>

using namespace pq;

#define N_METRO 3
Metro* metro[N_METRO] = {
  new Metro(0.01),
  new Metro(0.1),
  new Metro(0.1)
};

SquareOsc* osc[N_METRO] = {
  new SquareOsc(0.01),
  new SquareOsc(0.1),
  new SquareOsc(0.1)
};

#define RAMP_TIME 5.0f

Ramp testRamp;

testing(metro) {
  static float startTime = Plaquette.seconds();

  static float nMetro[N_METRO] = { 0.0f, 0.0f, 0.0f };
  // static float nMetro[N_METRO] = { 0.0f , 0.0f};
  // static float nMetro[N_METRO] = { 0.0f };
  static const float TOTAL_DURATION = 5.0f;

  Plaquette.step();

  for (int i=0; i<N_METRO; i++) {
    Metro* unit = metro[i];
    SquareOsc *unit2 = osc[i];
    // if (i == 0) {
    //   print(Plaquette.seconds()-startTime); print(" "); print(unit->isOn()); print(" "); print(unit2->get());
    //   if (unit->isOn())
    //     print(" ***");
    //   println();
    // }
    if (unit->isOn()) {
      nMetro[i]++;
      // assertEqual(unit2->get(), 1.0f);
    }
    if (unit->rose() && i != 2) {
      assertEqual(unit2->get(), 1.0f);
    }
  }

  if (Plaquette.seconds() - startTime > TOTAL_DURATION) {
    for (int i=0; i<N_METRO; i++) {
      Metro* unit = metro[i];
      assertNear(nMetro[i], (float)(TOTAL_DURATION/unit->period()), 4.0f);
      pass();
    }
  }
}

// testing(ramp) {
//   static float startTime = Plaquette.seconds();

//   assertMoreOrEqual(testRamp.get(), 0.0f);
//   assertLessOrEqual(testRamp.get(), 100.0f);
//   assertNear(testRamp.get(), mapFrom01((Plaquette.seconds()-startTime)/RAMP_TIME, 0, 100), 0.1f);
  
//   if (Plaquette.seconds() - startTime > RAMP_TIME) {
//     pass();
//   }

// }


void setup() {
  Plaquette.begin();
  for (int i=0; i<N_METRO; i++) {
    SquareOsc *unit2 = osc[i];
    unit2->phase(-0.25f);
    unit2->dutyCycle(0.75f);
  }
  testRamp.start(0, 100, RAMP_TIME);
  Plaquette.sampleRate(100000.0f);
}

void loop() {
  aunit::TestRunner::run();
}
