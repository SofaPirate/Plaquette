#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>

using namespace pq;

Engine engine1;
Engine engine2;

Metronome metro0(0.1);
Metronome metro1(0.1, engine1);
Metronome metro2(0.1, engine2);

SquareWave dummy0(1.0);
SquareWave dummy1(1.0, engine2);
SquareWave dummy2(1.0);

int count0 = 0;
int count1 = 0;
int count2 = 0;

testing(count) {
  static float startTime = -1;
  static const float RUNTIME = 5.0f;
  if (startTime < 0)
    startTime = Plaquette.seconds();

  Plaquette.step();
  engine1.step();
  engine2.step();

  if (metro0) count0 ++;
  if (metro1) count1 ++;
  if (metro2) count2 ++;

  if (Plaquette.seconds() - startTime > RUNTIME) {
    assertEqual(count0, (int)(RUNTIME/metro0.period())-1);
    assertEqual(count1, (int)(RUNTIME/metro1.period())-1);
    assertEqual(count2, (int)(RUNTIME/metro2.period())-1);
    pass();
  }

}

test(nUnits) {
  assertEqual((int)Plaquette.nUnits(), 3);
  assertEqual((int)engine1.nUnits(), 1);
  assertEqual((int)engine2.nUnits(), 2);
}

void setup() {
  Plaquette.begin();
  engine1.begin();
  engine2.begin();
}

void loop() {
  aunit::TestRunner::run();
}
