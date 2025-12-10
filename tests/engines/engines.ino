#include <Arduino.h>
#include <PlaquetteLib.h>
#include <AUnit.h>

using namespace pq;

Engine engine1;
Engine engine2;
Engine engineLowerSamplingRate;

Engine engineCustomTimeFunction;

Metronome metro0(0.1);
Metronome metro1(0.1, engine1);
Metronome metro2(0.1, engine2);

Wave dummy0(1.0);
Wave dummy1(1.0, engine2);
Wave dummy2(1.0);

Metronome metro3(1, engineCustomTimeFunction);

int count0 = 0;
int count1 = 0;
int count2 = 0;
int countLowerSamplingRate = 0;

#define TARGET_SAMPLE_RATE 10

testing(count) {
  static float startTime = -1;
  static const float RUNTIME = 5.0f;
  if (startTime < 0) {
    startTime = Plaquette.seconds();
  }

  Plaquette.step();
  engine1.step();
  engine2.step();

  if (metro0) count0 ++;
  if (metro1) count1 ++;
  if (metro2) count2 ++;

  if (Plaquette.seconds() - startTime >= RUNTIME) {
    assertNear(count0, (int)(RUNTIME/metro0.period()), 1);
    assertNear(count1, (int)(RUNTIME/metro1.period()), 1);
    assertNear(count2, (int)(RUNTIME/metro2.period()), 1);
    pass();
  }
}

testing(sampleRate) {
  static float startTime = -1;
  static const float RUNTIME = 5.0f;
  if (startTime < 0)
    startTime = engineLowerSamplingRate.seconds();

  if (engineLowerSamplingRate.step()) {
    countLowerSamplingRate++;
    assertNear(engineLowerSamplingRate.sampleRate(), (float)TARGET_SAMPLE_RATE, 0.5f);
  }

  if (engineLowerSamplingRate.seconds() - startTime >= RUNTIME) {
    assertNear((float)countLowerSamplingRate, (float)(RUNTIME*TARGET_SAMPLE_RATE), 2.0f);
    pass();
  }
}

unsigned long customMicros = 0;

unsigned long customMicroSeconds() { return customMicros; }

int customTimeFunctionMetroCount = 0;

testing(customTimeFunction) {
  if (customMicros < 10000000UL) {
    engineCustomTimeFunction.step();
    if (metro3.isOn()) {
      // test matching to a precision of 100 us
      assertTrue((customMicros / 100 + 1) % 1000 == 0);
      customTimeFunctionMetroCount ++;
    }

    customMicros ++;
  }
  else {
    assertEqual(customTimeFunctionMetroCount, 10);
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
  engineLowerSamplingRate.begin();
  engineLowerSamplingRate.sampleRate(TARGET_SAMPLE_RATE);

  engineCustomTimeFunction.referenceClock(customMicroSeconds);
  engineCustomTimeFunction.begin();
}

void loop() {
  aunit::TestRunner::run();
}
